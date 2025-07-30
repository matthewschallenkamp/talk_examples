from operator import neg

def find_qualifying_players(usernames, scores, reaction_times, min_score, max_time):
    return list(zip(*sorted(filter(
        lambda p: -p[0] >= min_score and p[1] <= max_time, 
        zip(map(neg, scores), reaction_times, usernames)
    ))))[2]
