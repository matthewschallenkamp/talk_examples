def process_user_data(user_id):
    """
    BUGGY VERSION: Race condition between read and transaction
    The row could be modified by another process between the read
    and when the transaction actually starts.
    """
    # Read the current row
    row = db.fetch_row("SELECT * FROM users WHERE id = ?", user_id)
    
    if not row:
        return None
    
    # BUG: Gap between reading the row and starting transaction
    # Another process could modify this row here!
    result = transmogrify_row(row)
    
    return result

def transmogrify_row(row):
    """
    This function starts its own transaction, but the row data
    might already be stale by the time we get here.
    """
    with db.transaction() as tx:
        # Apply some business logic transformation
        new_balance = row['balance'] + calculate_interest(row)
        new_status = 'active' if new_balance > 0 else 'inactive'
        
        # Update the row - but this might conflict with concurrent changes!
        tx.execute(
            "UPDATE users SET balance = ?, status = ? WHERE id = ?",
            new_balance, new_status, row['id']
        )
        
        return {'balance': new_balance, 'status': new_status}