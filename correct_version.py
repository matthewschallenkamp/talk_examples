def process_user_data(user_id):
    """
    CORRECT VERSION: Transaction is created at the right level
    The row is read WITHIN the transaction, ensuring consistency.
    """
    with db.transaction() as tx:
        # Read the row WITHIN the transaction for consistency
        row = tx.fetch_row("SELECT * FROM users WHERE id = ?", user_id)
        
        if not row:
            return None
        
        # Now pass both the fresh row data AND the transaction
        result = transmogrify_row_with_tx(row, tx)
        
        return result

def transmogrify_row(row):
    """
    Legacy version - now properly creates transaction at the right scope
    and reads fresh data within that transaction.
    """
    # For backward compatibility, if called without a transaction,
    # we need to create one and read fresh data
    with db.transaction() as tx:
        # Re-read the row within our transaction to ensure freshness
        fresh_row = tx.fetch_row("SELECT * FROM users WHERE id = ?", row['id'])
        
        if not fresh_row:
            raise ValueError(f"Row with id {row['id']} no longer exists")
        
        return transmogrify_row_with_tx(fresh_row, tx)

def transmogrify_row_with_tx(row, tx):
    """
    FINAL VERSION: Always expects a transaction and fresh row data
    This is now safe from race conditions.
    """
    if tx is None:
        raise ValueError("Transaction must be provided")
    
    # Apply business logic transformation using fresh data
    new_balance = row['balance'] + calculate_interest(row)
    new_status = 'active' if new_balance > 0 else 'inactive'
    
    # Update the row within the same transaction that read it
    tx.execute(
        "UPDATE users SET balance = ?, status = ? WHERE id = ?",
        new_balance, new_status, row['id']
    )
    
    return {'balance': new_balance, 'status': new_status}

def calculate_interest(row):
    """Helper function for business logic"""
    return row['balance'] * 0.02  # 2% interest