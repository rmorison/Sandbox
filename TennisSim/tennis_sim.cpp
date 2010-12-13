
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double drand()
{
    const int range = 1000000;

    return (rand() % range) / (double)range;
}

const char* human_score(int player1_score, int player2_score)
{
    const char* deuce[] = { "player1 add", "deuce", "player2_add" };
    const char* game[] = { "love", "15", "30", "40" };
    static char buf[512];

    if (player1_score >= 3 && player2_score >= 3)
    {
	int diff = player1_score - player2_score;
	if (diff > 1)
	    return "player1 win";
	if (diff < -1)
	    return "player2 win";
	sprintf(buf, "%s", deuce[1-diff]);
    } else {
	if (player1_score > 3)
	    return "player1 win";
	if (player2_score > 3)
	    return "player2 win";
	sprintf(buf, "%s-%s", game[player1_score], game[player2_score]);
    }
    return buf;
}

int play_a_game(double player1_advantage = 0.0, bool verbose = false)
{
    int player1_score = 0;
    int player2_score = 0;

    while (true)
    {
	if (drand() < 0.5 + player1_advantage)
	    player1_score += 1;
	else
	    player2_score += 1;
	if (verbose)
	    printf("    %s\n", human_score(player1_score, player2_score));
	//printf("    %d-%d\n", player1_score, player2_score);
	if (player1_score >= 4 && player1_score - player2_score >= 2)
	    return 1;
	if (player2_score >= 4 && player2_score - player1_score >= 2)
	    return 2;
    }
}

int play_a_set(double player1_advantage = 0.0, bool verbose = false)
{
    int player1_games = 0;
    int player2_games = 0;

    while (true)
    {
	int winner = play_a_game(player1_advantage, verbose);
	player1_games += winner == 1 ? 1 : 0;
	player2_games += winner == 2 ? 1 : 0;
	if (verbose)
	    printf("  %d-%d\n", player1_games, player2_games);
	if (player1_games >= 6 and player1_games - player2_games >= 2)
	    return 1;
	if (player2_games >= 6 and player2_games - player1_games >= 2)
	    return 2;
    }
    
}

bool strsame(const char* s1, const char* s2)
{
    return 0 == strcmp(s1, s2);
}

void usage(const char* prog)
{
    printf("%s: [-v] [-a player1_percent_advantage] nsets\n", prog);
}

int main(int argc, const char* argv[])
{
    bool verbose = false;
    int nsets = 0;
    double player1_advantage = 0.0;

    for(int arg = 0; arg < argc; arg += 1)
    {
	if (strsame(argv[arg], "-v"))
	{
	    verbose = true;
	} else if (strsame(argv[arg], "-a")) {
	    double percent = 0.0;
	    if (arg + 1 >= argc || 1 != sscanf(argv[arg+1], "%lf", &percent))
	    {
		usage(argv[0]);
		exit(1);
	    }
	    player1_advantage = percent / 100.0;
	} else {
	    sscanf(argv[arg], "%d", &nsets);
	}
    }

    if (! nsets) {
	usage(argv[0]);
	exit(1);
    }

    srand( time(NULL) );
    int player1_sets = 0;
    int player2_sets = 0;
    for(int set_num = 0; set_num < nsets; set_num += 1)
    {
	int winner = play_a_set(player1_advantage, verbose);
	player1_sets += winner == 1 ? 1 : 0;
	player2_sets += winner == 2 ? 1 : 0;
	if (verbose)
	    printf("player %d wins set %d\n", winner, set_num+1);
    }
    printf("player1 has %.2f%% advantage over player2\n", 100*player1_advantage);
    printf("player1 won %d of %d sets, %.2f%%\n", player1_sets, nsets, 100*player1_sets/(double)nsets);
    printf("player2 won %d of %d sets, %.2f%%\n", player2_sets, nsets, 100*player2_sets/(double)nsets);

}
