    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "Players_data.h"

    typedef struct NPlayer{
        int id;
        char* name;
        char* team;
        char* role;
        int totalRuns;
        float battingAverage;
        float strikeRate;
        int wickets;
        float economyRate;
        float performanceIndex;

        struct NPlayer *nextPlayer;
    } NPlayer;

    typedef struct NTeam{

        int teamID;
        const char *teamName;
        int totalPlayers;
        float totalStrikeRate;
        int battingPlayerCount;
        float avgBattingStrikeRate;

        struct NPlayer *batsmenHead;
        struct NPlayer *bowlerHead;
        struct NPlayer *allRounderHead;

    } NTeam;

    int addPlayerToATeam(NTeam teams[10],int idValid[]){
        char buffer[128];
        char nameBuf[128];
        char extra;
        int teamId;
        int playerId;
        int roleSel;
        int totalRuns;
        float battingAverage;
        float strikeRate;
        int wickets;
        float economyRate;

        while (1) {
            printf("Enter Team ID to add player: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
            if (sscanf(buffer, "%d %c", &teamId, &extra) == 1 && teamId >= 0 && teamId <= 9) break;
            fprintf(stderr, "Please enter a valid Team ID (0-9)\n");
        }

        printf("Enter Player Details:\n");

        while (1) {
            printf("Player ID: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
            if (sscanf(buffer, "%d %c", &playerId, &extra) == 1 && playerId >= 1 && playerId <= 1500){
                if(idValid[playerId] == 0){
                    break;
                }
                else{
                    fprintf(stderr,"Entered ID is already taken ");
                }
            }
            else{
                fprintf(stderr, "Please enter a valid Player ID (1-1000)\n");
            }
        }
        idValid[playerId] = 1;

        while (1) {
            printf("Name: ");
            if (!fgets(nameBuf, sizeof(nameBuf), stdin)) return -1;
            size_t ln = strlen(nameBuf);
            if (ln > 0 && nameBuf[ln-1] == '\n') { nameBuf[ln-1] = '\0'; ln--; }
            if (ln >= 1 && ln <= 50) break;
            fprintf(stderr, "Please enter a valid name (1-50 characters)\n");
        }

        while (1) {
            printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
            if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
            if (sscanf(buffer, "%d %c", &roleSel, &extra) == 1 && (roleSel >= 1 && roleSel <= 3)) break;
            fprintf(stderr, "Please enter 1, 2 or 3 for role\n");
        }

        while (1) {
            printf("Total Runs: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
            if (sscanf(buffer, "%d %c", &totalRuns, &extra) == 1 && totalRuns >= 0) break;
            fprintf(stderr, "Please enter a non-negative integer for Total Runs\n");
        }

        while (1) {
            printf("Batting Average: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
            if (sscanf(buffer, "%f %c", &battingAverage, &extra) == 1 && battingAverage >= 0.0f) break;
            fprintf(stderr, "Please enter a non-negative number for Batting Average\n");
        }

        while (1) {
            printf("Strike Rate: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
            if (sscanf(buffer, "%f %c", &strikeRate, &extra) == 1 && strikeRate >= 0.0f) break;
            fprintf(stderr, "Please enter a non-negative number for Strike Rate\n");
        }

        while (1) {
            printf("Wickets: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
            if (sscanf(buffer, "%d %c", &wickets, &extra) == 1 && wickets >= 0) break;
            fprintf(stderr, "Please enter a non-negative integer for Wickets\n");
        }

        while (1) {
            printf("Economy Rate: ");
            if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
            if (sscanf(buffer, "%f %c", &economyRate, &extra) == 1 && economyRate >= 0.0f) break;
            fprintf(stderr, "Please enter a non-negative number for Economy Rate\n");
        }

        NPlayer *newPlayer = malloc(sizeof(NPlayer));
        
        newPlayer->id = playerId;
        newPlayer->name = strdup(nameBuf);
        newPlayer->team = teams[teamId].teamName;
        
        switch(roleSel){
            case 1: newPlayer->role = "Batsman"; break;
            case 2: newPlayer->role = "Bowler"; break;
            case 3: newPlayer->role = "All-rounder"; break;
        }
        
        newPlayer->totalRuns = totalRuns;
        newPlayer->battingAverage = battingAverage;
        newPlayer->strikeRate = strikeRate;
        newPlayer->wickets = wickets;
        newPlayer->economyRate = economyRate;
        newPlayer->nextPlayer = NULL;
        float performanceIndex = 0.0f;
        if(strcmp(newPlayer->role,"Batsman") == 0){
            performanceIndex = (newPlayer->battingAverage * newPlayer->strikeRate)/100;
            newPlayer->performanceIndex = performanceIndex;
        }
        else if(strcmp(newPlayer->role,"All-rounder") == 0){
            performanceIndex = ((newPlayer->battingAverage*newPlayer->strikeRate)/100) + (newPlayer->wickets*2);
            newPlayer->performanceIndex = performanceIndex;
        }
        else if(strcmp(newPlayer->role,"Bowler") == 0){
            performanceIndex = (newPlayer->wickets*2) + (100 - newPlayer->economyRate);
            newPlayer->performanceIndex = performanceIndex;
        }
        addPlayerToTeam(newPlayer, teamId, teams);
        printf("Player data collected successfully for Team ID %d\n", teamId);
        return teamId;
    }

    void printMenu(){
        printf("========================================\n");
        printf("  ICC ODI Player Performance Analyzer\n");
        printf("========================================\n");
        printf("1. Add Player to team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");
    }

    void initializeTeamsArray(NTeam teamsa[10]){
        for(int teamIndex=0;teamIndex<10;teamIndex++){
            teamsa[teamIndex].teamID = teamIndex;
            teamsa[teamIndex].teamName = teams[teamIndex];
            teamsa[teamIndex].batsmenHead = NULL;
            teamsa[teamIndex].bowlerHead = NULL;
            teamsa[teamIndex].allRounderHead = NULL;
            teamsa[teamIndex].totalPlayers = 0;
            teamsa[teamIndex].battingPlayerCount = 0;
            teamsa[teamIndex].totalStrikeRate = 0.0f;
            teamsa[teamIndex].avgBattingStrikeRate = 0.0f;
        }
    }
    void addPlayer(Player player,NTeam team[10],int index){
        NPlayer *newPlayer = malloc(sizeof(NPlayer));
        newPlayer->id = player.id;
        newPlayer->name = strdup(player.name);
        newPlayer->team = player.team;
        newPlayer->role = player.role;
        newPlayer->totalRuns = player.totalRuns;
        newPlayer->battingAverage = player.battingAverage;
        newPlayer->strikeRate = player.strikeRate;
        newPlayer->wickets = player.wickets;
        newPlayer->economyRate = player.economyRate;

        float performanceIndex = 0.0f;
        if(strcmp(player.role,"Batsman") == 0){
            performanceIndex = (player.battingAverage * player.strikeRate)/100;
            newPlayer->performanceIndex = performanceIndex;
        }
        else if(strcmp(player.role,"All-rounder") == 0){
            performanceIndex = ((player.battingAverage*player.strikeRate)/100) + (player.wickets*2);
            newPlayer->performanceIndex = performanceIndex;
        }
        else if(strcmp(player.role,"Bowler") == 0){
            performanceIndex = (player.wickets*2) + (100 - player.economyRate);
            newPlayer->performanceIndex = performanceIndex;
        }

        newPlayer->nextPlayer = NULL;
        addPlayerToTeam(newPlayer,index,team);
    }
    NPlayer* addNodeToLList(NPlayer* head,NPlayer *player){
        NPlayer *temp = head;
        if(head == NULL || player->performanceIndex > head->performanceIndex){
            player->nextPlayer = head;
            return player;
        }
        NPlayer *current = head;
        while(current->nextPlayer != NULL && current->nextPlayer->performanceIndex > player->performanceIndex){
            current = current->nextPlayer;
        }
        player->nextPlayer = current->nextPlayer;
        current->nextPlayer = player;

        return head;
    }
    void addPlayerToTeam(NPlayer *player,int teamIndex,NTeam team[10]){
        team[teamIndex].totalPlayers++;
        if(strcmp(player->role,"Batsman") == 0 || strcmp(player->role,"All-rounder") == 0){
            team[teamIndex].totalStrikeRate += player->strikeRate;
            team[teamIndex].battingPlayerCount++;
            if(team[teamIndex].battingPlayerCount > 0){
                team[teamIndex].avgBattingStrikeRate = (team[teamIndex].totalStrikeRate / team[teamIndex].battingPlayerCount);
            }
        } 
        if(strcmp(player->role,"Bowler") == 0){
            team[teamIndex].bowlerHead = addNodeToLList(team[teamIndex].bowlerHead,player);
        }
        else if(strcmp(player->role,"Batsman") == 0){
            team[teamIndex].batsmenHead = addNodeToLList(team[teamIndex].batsmenHead,player);
        }
        else if(strcmp(player->role,"All-rounder") == 0){
            team[teamIndex].allRounderHead = addNodeToLList(team[teamIndex].allRounderHead,player);
        }
    }

    void loadInitialData(NTeam teams[10],int idValid[]){
        for(int index=0;index<playerCount;index++){
            if (players[index].id > 0 && players[index].id <= 1500) {
              idValid[players[index].id] = 1;
            }
            if(strcmp(players[index].team,"Afghanistan") == 0){
                addPlayer(players[index],teams,0);
            }else if (strcmp(players[index].team,"Australia") == 0){
                addPlayer(players[index],teams,1);
            }else if (strcmp(players[index].team,"Bangladesh") == 0){
                addPlayer(players[index],teams,2);
            }else if (strcmp(players[index].team,"England") == 0){
                addPlayer(players[index],teams,3);
            }else if (strcmp(players[index].team,"India") == 0){
                addPlayer(players[index],teams,4);
            }else if (strcmp(players[index].team,"New Zealand") == 0){
                addPlayer(players[index],teams,5);
            }else if (strcmp(players[index].team,"Pakistan") == 0){
                addPlayer(players[index],teams,6);
            }else if (strcmp(players[index].team,"South Africa") == 0){
                addPlayer(players[index],teams,7);
            }else if (strcmp(players[index].team,"Sri Lanka") == 0){
                addPlayer(players[index],teams,8);
            }else if (strcmp(players[index].team,"West Indies") == 0){
                addPlayer(players[index],teams,9);
            }        
        }
        printf("Initial Data is Loaded");
    }
    void displayPlayersofTeam(NTeam team[],int teamIndex){
        printf("Players of team %s\n", team[teamIndex].teamName);
        printf("======================================================\n");
        printf("ID\tName\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerfIndex\n");
        printf("======================================================\n");

        NPlayer *p = team[teamIndex].batsmenHead;
        while (p != NULL) {
            printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
                p->id, p->name, p->role, p->totalRuns, p->battingAverage,
                p->strikeRate, p->wickets, p->economyRate, p->performanceIndex);
                p = p->nextPlayer;
        }

        p = team[teamIndex].allRounderHead;
        while (p != NULL) {
            printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
                p->id, p->name, p->role, p->totalRuns, p->battingAverage,
                p->strikeRate, p->wickets, p->economyRate, p->performanceIndex);
                p = p->nextPlayer;
        }

        p = team[teamIndex].bowlerHead;
        while (p != NULL) {
            printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
                p->id, p->name, p->role, p->totalRuns, p->battingAverage,
                p->strikeRate, p->wickets, p->economyRate, p->performanceIndex);
                p = p->nextPlayer;
        }

        printf("========================================\n");
        printf("Total Players %d",team[teamIndex].totalPlayers);
        printf("Average Batting Strike Rate %.2f",team[teamIndex].avgBattingStrikeRate);
    }

    void topKPlayers(NTeam teamss[],int numPlayers,int teamID,int role){
        NPlayer *temp;
        if(role == 1){
            temp = teamss[teamID].batsmenHead;
        }
        else if(role == 2){
            temp = teamss[teamID].bowlerHead;
        }else{
            temp = teamss[teamID].allRounderHead;
        }
        printf("Top %d Players of team %s :\n",numPlayers,teamss[teamID].teamName);
        printf("======================================================\n");
        printf("ID\tName\tRole\tRuns\tAvg\tSR\tWkts\tER\tPerfIndex\n");
        printf("======================================================\n");
        while (numPlayers-- > 0 && temp != NULL)
        {  
            printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%d\t%.2f\t%.2f\n",
                temp->id, temp->name, temp->role, temp->totalRuns, temp->battingAverage,
                temp->strikeRate, temp->wickets, temp->economyRate, temp->performanceIndex);
                temp = temp->nextPlayer;
        }
    }
    void heapifyDown(NPlayer* heap[], int size, int index) {
        int largest = index;
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;

        if (leftChild < size && heap[leftChild]->performanceIndex > heap[largest]->performanceIndex) {
            largest = leftChild;
        }

        if (rightChild < size && heap[rightChild]->performanceIndex > heap[largest]->performanceIndex) {
            largest = rightChild;
        }

        if (largest != index) {
            NPlayer* temp = heap[index];
            heap[index] = heap[largest];
            heap[largest] = temp;
            heapifyDown(heap, size, largest);
        }
    }

    void buildMaxHeap(NPlayer* heap[], int size) {
        for (int i = (size / 2) - 1; i >= 0; i--) {
            heapifyDown(heap, size, i);
        }
    }
    void allPlayersOfRole(NTeam teamss[],int role){
        NPlayer* heap[10];
        int heapSize = 0;

        for (int i = 0; i < 10; i++) {
            NPlayer* listHead = NULL;
            if (role == 1) {
                listHead = teamss[i].batsmenHead;
            } else if (role == 2) {
                listHead = teamss[i].bowlerHead;
            } else {
                listHead = teamss[i].allRounderHead;
            }

            if (listHead != NULL) {
                heap[heapSize] = listHead;
                heapSize++;
            }
        }
        buildMaxHeap(heap, heapSize);
        const char* roleName = (role == 1) ? "Batsmen" : (role == 2) ? "Bowlers" : "All-rounders";
        printf("\n--- All %s Sorted by Performance Index ---\n", roleName);
        printf("==================================================================================\n");
        printf("%s\t %s\t %s\t %s\t %s\n", 
            "Rank", "Name", "Role", "Team", "Perf. Index");
        printf("==================================================================================\n");

        int rank = 1;
        while (heapSize > 0) {
            NPlayer* bestPlayer = heap[0];
            printf("%-5d %-20s %-12s %-15s %-10.2f\n", 
                rank++, 
                bestPlayer->name, 
                bestPlayer->role,
                bestPlayer->team,
                bestPlayer->performanceIndex);
            NPlayer* nextPlayer = bestPlayer->nextPlayer;

            if (nextPlayer != NULL) {
                heap[0] = nextPlayer;
            } else {
                heap[0] = heap[heapSize - 1];
                heapSize--;
            }
            heapifyDown(heap, heapSize, 0);
        }
        printf("==================================================================================\n\n");
    }

    void displaySortedTeams(NTeam teamss[]){
        NTeam sortedTeam[10];
        for(int i = 0; i < 10; i++) {
            sortedTeam[i] = teamss[i];
        }
        for(int index=0;index<10;index++){
            for(int jindex=0;jindex<10-index-1;jindex++){
                if(sortedTeam[jindex].avgBattingStrikeRate < sortedTeam[jindex+1].avgBattingStrikeRate){
                    NTeam tempTeam = sortedTeam[jindex];
                    sortedTeam[jindex] = sortedTeam[jindex+1];
                    sortedTeam[jindex+1] = tempTeam;
                }
            }
        }
        printf("Teams Sorted By Average Batting Strike Rate :\n");
        printf("======================================================\n");
        printf("ID\tTeam Name\t\t Avg Bat SR \t totalPlayers\n");
        printf("======================================================\n");
        for(int teamID=0;teamID<10;teamID++){
            printf("%-5d %-20s %-10.2f %d\n",
                        teamID + 1,
                        sortedTeam[teamID].teamName,
                        sortedTeam[teamID].avgBattingStrikeRate,
                        sortedTeam[teamID].totalPlayers
            );
        }
    }
    void freeList(NPlayer *head) {
        NPlayer *current = head;
        NPlayer *nextNode;
        while (current != NULL) {
            nextNode = current->nextPlayer;
            free(current->name);
            free(current);
            current = nextNode;
        }
    }

    void freeAll(NTeam teams[10]) {
        printf("\nFreeing all allocated memory...\n");
        for (int i = 0; i < 10; i++) {
            freeList(teams[i].batsmenHead);
            freeList(teams[i].bowlerHead);
            freeList(teams[i].allRounderHead);
        }
        printf("Cleanup complete. Exiting programm.\n");
    }
    int main(){
        int idValid[1501] = {0};
        NTeam teamS[10] = {0};
        initializeTeamsArray(teamS);
        loadInitialData(teamS,idValid);

        while (1)
        {
            printMenu();
            printf("Enter Your Choice: ");
            char buffer[10];
            int input;
            char bufferChar;
            fgets(buffer,sizeof(buffer),stdin);
            int inputChar = sscanf(buffer,"%d %c",&input,&bufferChar);
            if(inputChar != 1 || (inputChar == 2 && bufferChar != '\n')){
                fprintf(stderr,"Enter the correct choice");
            }
            if(input == 1){
                int teamId = addPlayerToATeam(teamS,idValid);
                if (teamId != -1) {
                    printf("\nSUCCESS: Player added to team %s\n\n", teamS[teamId].teamName);
                }
            }
            else if(input == 2){
                char buffer[10];
                int teamIndex;
                char bufferChar;
                printf("Enter the team ID: \n");
                while(1){
                    fgets(buffer,sizeof(buffer),stdin);
                    int scannedItems = sscanf(buffer,"%d %c",&teamIndex,&bufferChar);
                    if(scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n') && (teamIndex >= 0 && teamIndex <= 9)){
                        break;
                    }else{
                        fprintf(stderr,"please enter the valid team ID(0-9): \n");
                    }
                }
                displayPlayersofTeam(teamS,teamIndex);
            }
            else if(input == 3){
            displaySortedTeams(teamS);
            }
            else if(input == 4){
                char buffer[10];
                int teamIndex;
                int role;
                int numberPlayers;
                char bufferChar;
                printf("Enter the team ID: \n");
                while(1){
                    fgets(buffer,sizeof(buffer),stdin);
                    int scannedItems = sscanf(buffer,"%d %c",&teamIndex,&bufferChar);
                    if(scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n') && (teamIndex >= 0 && teamIndex <= 9)){
                        break;
                    }else{
                        fprintf(stderr,"please enter the valid team ID(0-9): \n");
                    }
                }
                printf("Enter the Role (1-Batsman,2-Bowler,3-All-rounder): \n");
                while(1){
                    fgets(buffer,sizeof(buffer),stdin);
                    int scannedItems = sscanf(buffer,"%d %c",&role,&bufferChar);
                    if(scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n') && (role == 1 || role == 2|| role == 3)){
                        break;
                    }else{
                        fprintf(stderr,"please enter the valid role: \n");
                    }
                }
                printf("Enter the Number of players: \n");
                while(1){
                    fgets(buffer,sizeof(buffer),stdin);
                    int scannedItems = sscanf(buffer,"%d %c",&numberPlayers,&bufferChar);
                    if(scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')){
                        break;
                    }else{
                        fprintf(stderr,"please enter the valid number: \n");
                    }
                }
                topKPlayers(teamS, numberPlayers, teamIndex, role);
            }
            else if(input == 5){
                char buffer[10];
                int role;
                printf("Enter the role (1-Batsman,2-Bowler,3-All-rounder): \n");
                while(1){
                    fgets(buffer,sizeof(buffer),stdin);
                    int scannedItems = sscanf(buffer,"%d %c",&role,&bufferChar);
                    if(scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n') && (role == 1 || role == 2|| role == 3)){
                        break;
                    }else{
                        fprintf(stderr,"please enter the valid role: \n");
                    }
                }
                allPlayersOfRole(teamS,role);
            }
            else if(input == 6){
                freeAll(teamS);
                break;
            }
            else{
                fprintf(stderr,"Invalid choice please enter the correct option \n");
            }
        }
    }