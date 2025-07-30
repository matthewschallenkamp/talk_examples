def find_qualifying_players_v1(usernames, scores, reaction_times, min_score, max_time):
    # Create lookup dictionaries to avoid keeping indices around
    score_lookup = {}
    time_lookup = {}
    qualifying_usernames = []
    
    # Build lookup dictionaries and find qualifying players
    for i in range(len(usernames)):
        username = usernames[i]
        score = scores[i]
        time = reaction_times[i]
        
        score_lookup[username] = score
        time_lookup[username] = time
        
        if (score >= min_score and 
            time <= max_time):
            qualifying_usernames.append(username)
    
    # Sort by looking up data in our dictionaries
    qualifying_usernames.sort(
        key=lambda username: (-score_lookup[username], time_lookup[username])
    )
    
    return qualifying_usernames
