def find_qualifying_players_v2(usernames, scores, reaction_times, min_score, max_time):
    # Filter and collect data with smart tuple ordering for natural sorting
    qualifying_players = [
        (-score, time, username)  # Negative score for desc sort, time for asc sort
        for username, score, time in zip(usernames, scores, reaction_times)
        if score >= min_score 
        and time <= max_time
    ]
    
    # Sort naturally - no lambda needed!
    qualifying_players.sort()
    
    # Extract usernames
    return [username for _, _, username in qualifying_players]
