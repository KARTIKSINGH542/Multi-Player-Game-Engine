// Problem Statement:
// There are three entity and four individuals (one[Guesser] , Three[Player] & one[Umpire])
// Guesser : This entity will guess a number.
// Players : Each of the three players will guess a number. 
// Umpire  : This entity will collect the guesses from the Guesser 
//          and the players and determine which Players, if any, have
//          guessed the same number as the Guesser.
#include<bits/stdc++.h>
using namespace std;

class Guesser{
    int guessed_number;
    public:
    int take_gueeser_input(){
        int g;
        cout<<"Give the number for the Guesser : ";
        cin>>g;
        guessed_number=g;
        return guessed_number;
    }
};

class Player{
    int player_number;
    public:
    int take_player_input(){
        int p;
        cin>>p;
        player_number=p;
        return player_number;
    }
};

class Umpire{
    public:
    int g_num;
    int p1_num;
    int p2_num;
    int p3_num;
    void get_number_from_guesser(){
        Guesser g1;
        g_num=g1.take_gueeser_input();
    }
    void get_number_from_player(){
        Player p1;
        Player p2;
        Player p3;
        cout<<"Give the number for the player 1: ";
        p1_num=p1.take_player_input();
        cout<<"Give the number for the player 2: ";
        p2_num=p2.take_player_input();
        cout<<"Give the number for the player 3: ";
        p3_num=p3.take_player_input();
    }
    void print_result(){
        vector<int> v(3,0);
        if(g_num==p1_num){
            v[0]=1;
        }
        if(g_num==p2_num){
            v[1]=1;
        }
        if(g_num==p3_num){
            v[2]=1;
        }
        if(v[0]==1 && v[1]==1 && v[2]==1){
            cout<<"All players has won the game";
        }
        else if(v[0]==1 && v[1]==1 && v[2]==0){
            cout<<"player1 and player2 has won the game";
        }
        else if(v[0]==1 && v[1]==0 && v[2]==1){
            cout<<"player1 and player3 has won the game";
        }
        else if(v[0]==0 && v[1]==1 && v[2]==1){
            cout<<"player2 and player3 has won the game";
        }
        else if(v[0]==1 && v[1]==0 && v[2]==0){
            cout<<"player1 has won the game";
        }
        else if(v[0]==0 && v[1]==1 && v[2]==0){
            cout<<"player2 has won the game";
        }
        else if(v[0]==0 && v[1]==0 && v[2]==1){
            cout<<"player3 has won the game";
        }
        else {
            cout<<"All players has lost the game";
        }
    }

};


int main(){
/* main function -> 1. Create a Umpire.
                    2. Get the number from Guesser.
                    3. Get the number from all the three Players.
                    4. Print the result of the game.
                    5. End the game
*/
cout<<"---------Let Start the Game----------"<<endl;

Umpire u;
u.get_number_from_guesser();
u.get_number_from_player();
u.print_result();

cout<<endl<<"---------------End Game---------------"<<endl;
}