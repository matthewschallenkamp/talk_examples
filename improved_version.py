def process_user_data(user_id):
    """
    IMPROVED VERSION: Still has the same fundamental issue
    We're still reading outside the transaction, then passing
    stale data to the transactional function.
    """
    # Still reading outside any transaction context
    row = db.fetch_row("SELECT * FROM users WHERE id = ?", user_id)
    
    if not row:
        return None
    
    # Now we pass the row to a transaction-aware version
    result = transmogrify_row_with_tx(row)
    
    return result

def transmogrify_row(row):
    """
    Legacy version - now delegates to the transaction-aware version
    This maintains backward compatibility while fixing the internal logic.
    """
    return transmogrify_row_with_tx(row)

def transmogrify_row_with_tx(row, tx=None):
    """
    NEW: Transaction-aware version that can accept an existing transaction
    But we still have the fundamental issue - the row data might be stale
    if it was read outside the transaction.
    """
    def do_transmogrify(transaction):
        # Apply business logic transformation
        new_balance = row['balance'] + calculate_interest(row)
        new_status = 'active' if new_balance > 0 else 'inactive'
        
        # Update the row
        transaction.execute(
            "UPDATE users SET balance = ?, status = ? WHERE id = ?",
            new_balance, new_status, row['id']
        )
        
        return {'balance': new_balance, 'status': new_status}
    
    if tx is not None:
        # Use the provided transaction
        return do_transmogrify(tx)
    else:
        # Create our own transaction
        with db.transaction() as new_tx:
            return do_transmogrify(new_tx)