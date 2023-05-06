#include"cpplibduc.h"
#include"MAPclass.h"

int main(){
    int A, B, diff;
    string DECOY;

    cout<<"\n\nStart the game? 'yes' or 'no': "; 
    ASKAGAIN:
    cin>>DECOY;
    if(DECOY=="no"){cout<<"\nGoodbye"; return 0;}
    if(DECOY!="yes" && DECOY!="no") {cout<<"\nJust type 'yes' or 'no': "; goto ASKAGAIN;}

    cout<<"\n\nDimension AxB.";
    A_Again:
    cout<<"\nA: ";
    cin>>A;
    if(A<10)
    {
        cout<<"\nA must be in [10,15]";
        goto A_Again;
    }
    B_Again:
    cout<<"\nB: ";
    cin>>B;
    if(B<10)
    {
        cout<<"\nB must be in [10,50]";
        goto B_Again;
    }

    cout<<"\nDifficulty? 'easy', 'normal' or 'hard': "; 
    Diff_Again:
    cin>>DECOY;
    if(DECOY=="easy") diff=1;
    else if(DECOY=="normal") diff=2;
    else if(DECOY=="hard") diff=3;
    else{cout<<"\nInvalid difficulty, type again: "; goto Diff_Again;}   

    Map MAP(A,B,diff);

    MAP.RandomMaze();
    //MAP.ShowCharMap();
    MAP.ShowMap();

    cout<<"Tap something to play: "; cin>>DECOY;

    MAP.Play();
    cout<<"\n\n";
}