
using namespace std;

class MazePart{
    private:
    /*                                                          
        PATH type      (    )                                   ()[][][]()[][][]()[][][]()
        f: first       (    )                                   [](    )[](    )[](    )[]
        p: path        (    )                                   [](    )[](    )[](    )[]
        e: end                                                  [](    )[](    )[](    )[]
    //                                                          ()[][][]()[][][]()[][][]()
        WALL type      [][][]            []                     [](    )[](    )[](    )[]
        h: hard wall (cannot penetrated) []                     [](    )[](    )[](    )[]
        w: wall                          []                     [](    )[](    )[](    )[]
        n: not wall                                             ()[][][]()[][][]()[][][]()
        d: door (same with not wall but in random creating process)

        DONT CARE type ()
        x: dont care
    */
    char type;
    
    public:
    MazePart(){
        type='0';
    }
    void SetType(char);
    char GetType();
};

class Map{
    private:
    MazePart **MAP;
    int height;
    int width;
    int difficulty;
    string PATH;

    int *ListX;
    int *ListY;

    public:
    Map(int, int, int);

    ~Map();

    void CreateNewList();
    void DeleteList();

    bool CheckLoop(int X, int Y);
    void DoorToNoWall();
    bool IsIsolated(int Y, int X);
    bool IsWall(int x, int y);
    bool IsPath(int x, int y);
    bool IsDontCare(int x, int y);
    char ToDir(int random);

    void RandomMaze();
        string RandomMainPath(int XStart, int YStart, int XEnd, int YEnd);
        bool Move(int &Y, int &X, char dir);
    
    void ShowMap();
    void ShowCharMap();

    void Play();
    void RefreshPlayer();

    //Test
    void TestMap();
};

void MazePart::SetType(char c){type=c;}
char MazePart::GetType(){return type;}

Map::Map(int A, int B, int diff){
    height=A*2+1;
    width=B*2+1;
    difficulty=diff;
    PATH="";
    MAP=new MazePart*[height];
    for(int i=0;i<height;i++)
        MAP[i]=new MazePart[width];

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(IsDontCare(i,j)) MAP[i][j].SetType('x');
            if(IsPath(i,j)) MAP[i][j].SetType('p');
            if(IsWall(i,j)) MAP[i][j].SetType('w');
            if((i==0 || i==height-1) && IsWall(i,j)) MAP[i][j].SetType('h');
            if((j==0 || j==width-1) && IsWall(i,j)) MAP[i][j].SetType('h');
        }
    }
}

Map::~Map(){
    for(int i=0;i<height;i++)
    {
        delete[] MAP[i];
    }
}

void Map::CreateNewList(){
    ListX=new int[2500];
    ListY=new int[2500];
    for(int i=0;i<2500;i++)
    {
        ListX[i]=0;
        ListY[i]=0;
    }
}

bool Map::CheckLoop(int X, int Y){
    int idx=0;
    while(idx<2500)
    {
        if(X==ListX[idx] && Y==ListY[idx]) return true;
        idx++;
    }
    return false;
}

void Map::DeleteList(){
    delete[] ListX;
    delete[] ListY;
}

void Map::DoorToNoWall(){
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
            if(MAP[i][j].GetType()=='d') MAP[i][j].SetType('n');
}

bool Map::IsIsolated(int y, int x){
    if(y%2!=1 || y%2!=1) return false;

    return (!(MAP[y+1][x].GetType()=='n')
            && !(MAP[y-1][x].GetType()=='n')
            && !(MAP[y][x+1].GetType()=='n')  
            && !(MAP[y][x-1].GetType()=='n'))?
            
    true:false;
}

bool Map::IsWall(int x, int y){return (x+y)%2==1?true:false;}

bool Map::IsPath(int x, int y){return (x%2==1 && y%2==1)?true:false;}

bool Map::IsDontCare(int x, int y){return (x%2==0 && y%2 ==0)?true:false;}

char Map::ToDir(int random){
    if(random==0) return 'w';
    if(random==1) return 'a';
    if(random==2) return 's';
    if(random==3) return 'd';
    return 'w';
}
bool Map::Move(int &Y, int &X, char dir){
    switch(dir)
    {
        case 'w': 
        {
            if(MAP[Y-1][X].GetType()=='h') return false;
            if(CheckLoop(X,Y-2)) return false;
            Y--;
            MAP[Y][X].SetType('d');
            Y--;
            break;
        }
        case 'a':
        {
            if(MAP[Y][X-1].GetType()=='h') return false;
            if(CheckLoop(X-2,Y)) return false;
            X--;
            MAP[Y][X].SetType('d');
            X--;
            break;
        }
        case 's':
        {
            if(MAP[Y+1][X].GetType()=='h') return false;
            if(CheckLoop(X,Y+2)) return false;
            Y++;
            MAP[Y][X].SetType('d');
            Y++;
            break;
        }
        case 'd':
        {
            if(MAP[Y][X+1].GetType()=='h') return false;
            if(CheckLoop(X+2,Y)) return false;
            X++;
            MAP[Y][X].SetType('d');
            X++;
            break;
        }
    }
    return true;
}

string Map::RandomMainPath(int XStart, int YStart, int XEnd, int YEnd){

    string OUTPUT;
    int X, Y, count, randomN;
    int DECOY;
    char c;
    bool moved;
    bool CanMove[4];

    StartAgain:
    CreateNewList();

    cout<<"!! STARTING FROM BEGIN !!";
    OUTPUT="";
    count=0;
    X=XStart;
    Y=YStart;

    ListX[count]=X;
    ListY[count]=Y;
    while(X!=XEnd || Y!=YEnd)
    {
        moved=false;

        for(int i=0;i<4;i++) CanMove[i]=true;

        while(!moved)
        {
            if(!CanMove[0] && !CanMove[1] && !CanMove[2] && !CanMove[3]) 
            {
                DeleteList();
                cout<<"\n\n!! Failed to get to the final destination !!\n";
                goto StartAgain;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            if(X<=XEnd)
            {
                c='d';
                if(Y<=YEnd){if(rand()%2==0) c='s';}
                else{if(rand()%2==0) c='w';}
            }
            if(X>=XEnd)
            {
                c='a';
                if(Y<=YEnd){if(rand()%2==0) c='s';}
                else{if(rand()%2==0) c='w';}
            }

            //if diff=1, 60% go diagonally at each step
            //if diff=2, 50% go diagonally at each step
            //if diff=3, 40% go diagonally at each step
            if(rand()%10<7-difficulty) ;
            else c=ToDir(rand()%4);
            
            switch(c)
            {
                case 'w':
                {
                    if(Y-2<0 || CheckLoop(X,Y-2))
                    {
                        CanMove[0]=false;
                        break;
                    }  
                    Y-=2;
                    OUTPUT+="w";
                    moved=true;
                    break;
                }
                case 's':
                {
                    if(Y+2>=height || CheckLoop(X,Y+2))
                    {
                        CanMove[1]=false;
                        break;
                    }  
                    Y+=2;
                    OUTPUT+="s";
                    moved=true;
                    break;
                }
                case 'a':
                {
                    if(X-2<0 || CheckLoop(X-2,Y))
                    {
                        CanMove[2]=false; 
                        break;
                    }  
                    X-=2;
                    OUTPUT+="a";
                    moved=true;
                    break;
                }
                case 'd':
                {
                    if(X+2>=width || CheckLoop(X+2,Y))
                    {
                        CanMove[3]=false; 
                        break;
                    }  
                    X+=2;
                    OUTPUT+="d";
                    moved=true;
                    break;
                }
            }

            cout<<"\n\nCreating main path... randomly find path to the end..."<<((double(X)/XEnd+double(Y)/YEnd)/2)*100<<"%";
        }
        
        count++;
        ListX[count]=X;
        ListY[count]=Y;
    }
    DeleteList();
    return OUTPUT;
}

void Map::RandomMaze(){
    int DECOY, XStart, YStart, XEnd, YEnd;
    string INPUT;
    
    XStart=1; YStart=1;
    XEnd=width-2; YEnd=height-2;

    MAP[YStart][XStart].SetType('s');
    MAP[YEnd][XEnd].SetType('e');

    int X=XStart, Y=YStart;

    //Randomize the main path from start to end(expressed in a string)
    PATH=RandomMainPath(XStart,YStart,XEnd,YEnd);
    
    cout<<"\n\nShow the solution? 'yes' or 'no': "; 
    TYPEAGAIN:
    cin>>INPUT;
    if(INPUT=="yes") cout<<"\nSolution: \""<<PATH<<"\""<<endl;
    if(INPUT!="yes" && INPUT!="no"){cout<<"\nJust type 'yes' or 'no': "; goto TYPEAGAIN;}

    //Create the main path in the Map using the string above
    for(int i=0;i<PATH.size();i++)
    {
        if(i==0 || i==1)
        {
            if(MAP[Y-1][X].GetType()!='n') MAP[Y-1][X].SetType('h');
            if(MAP[Y+1][X].GetType()!='n') MAP[Y+1][X].SetType('h');
            if(MAP[Y][X+1].GetType()!='n') MAP[Y][X+1].SetType('h');
            if(MAP[Y][X-1].GetType()!='n') MAP[Y][X-1].SetType('h');
        }
        switch(PATH[i])
        {
            case 'w': 
            {
                Y--;
                MAP[Y][X].SetType('n');
                Y--;
                break;
            }
            case 'a':
            {
                X--;
                MAP[Y][X].SetType('n');
                X--;
                break;
            }
            case 's':
            {
                Y++;
                MAP[Y][X].SetType('n');
                Y++;
                break;
            }
            case 'd':
            {
                X++;
                MAP[Y][X].SetType('n');
                X++;
                break;
            }
        }

        if(i==PATH.size()-1 || i==PATH.size()-2)
        {
            if(MAP[Y-1][X].GetType()!='n') MAP[Y-1][X].SetType('h');
            if(MAP[Y+1][X].GetType()!='n') MAP[Y+1][X].SetType('h');
            if(MAP[Y][X+1].GetType()!='n') MAP[Y][X+1].SetType('h');
            if(MAP[Y][X-1].GetType()!='n') MAP[Y][X-1].SetType('h');
        }
    }

    //Randomly connecting every isolated point on map to the main path
    int count;
    int SelfBlocked;
    for(int i=1;i<height;i+=2)
    {
        for(int j=1;j<width;j+=2)
        {
            if(IsIsolated(i,j))
            {
                SelfBlocked=0;
                count=0;

                CreateNewList();
                X=j;
                Y=i;
                ListX[count]=X;
                ListY[count]=Y;
                while(IsIsolated(Y,X))
                {
                    while(!Move(Y,X,ToDir(rand()%4))) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        SelfBlocked++;

                        if(SelfBlocked>20) break;
                    }

                    if(SelfBlocked>20) break;

                    count++;
                    ListX[count]=X;
                    ListY[count]=Y;
                }

                if(SelfBlocked>20) 
                {
                    j-=2;
                    continue;
                }

                cout<<"\nWait for it..."<<(((double(i)/height)*100+(double(j)/width)*10)<100?((double(i)/height)*100+(double(j)/width)*10):99.99)<<"%";

                DoorToNoWall();
                DeleteList();
            }
        }
    }
    cout<<"\n";

    //Finally, fixing the dont care point with no wall attatched to it
    for(int i=2;i<height-2;i++)
    {
        for(int j=2;j<width-2;j++)
        {
            if(IsDontCare(i,j) && (
                MAP[i+1][j].GetType()=='n'
            &&  MAP[i-1][j].GetType()=='n'
            &&  MAP[i][j-1].GetType()=='n'
            &&  MAP[i][j+1].GetType()=='n'
            ))
            {
                switch(rand()%4)
                {
                    case 0:
                    {
                        MAP[i+1][j].SetType('w');
                        break;
                    }
                    case 1:
                    {
                        MAP[i-1][j].SetType('w');
                        break;
                    }
                    case 2:
                    {
                        MAP[i][j-1].SetType('w');
                        break;
                    }
                    case 3:
                    {
                        MAP[i][j+1].SetType('w');
                        break;
                    }
                }
            }
        }
    }

    return;
}

void Map::ShowCharMap(){
    for(int i=0;i<width;i++)
    {
        cout<<setw(3)<<left<<i;
    }
    cout<<endl;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            cout<<MAP[i][j].GetType()<<"  ";
        }
        cout<<endl<<endl<<endl;
    }
}

void Map::ShowMap(){
    for(int j=0;j<width;j++)
    {
        if(MAP[0][j].GetType()=='x') cout<<"H";
        if(MAP[0][j].GetType()=='w') cout<<"HHH";
        if(MAP[0][j].GetType()=='h') cout<<"HHH";
    }
    cout<<endl;
    for(int i=1;i<height-1;)
    {
        for(int j=0;j<width;j++)
        {
            if(MAP[i][j].GetType()=='w') cout<<"H";
            if(MAP[i][j].GetType()=='h') cout<<"H";
            if(MAP[i][j].GetType()=='n') cout<<" ";
            if(MAP[i][j].GetType()=='d') cout<<" ";
            if(MAP[i][j].GetType()=='s') cout<<"   ";
            if(MAP[i][j].GetType()=='p') cout<<"   ";
            if(MAP[i][j].GetType()=='e') cout<<"   ";
            if(MAP[i][j].GetType()=='P') cout<<"   ";
        }
        cout<<endl;
        for(int j=0;j<width;j++)
        {
            if(MAP[i][j].GetType()=='w') cout<<"H";
            if(MAP[i][j].GetType()=='h') cout<<"H";
            if(MAP[i][j].GetType()=='n') cout<<" ";
            if(MAP[i][j].GetType()=='d') cout<<" ";
            if(MAP[i][j].GetType()=='s') cout<<" S ";
            if(MAP[i][j].GetType()=='p') cout<<"   ";
            if(MAP[i][j].GetType()=='e') cout<<" E ";
            if(MAP[i][j].GetType()=='P') cout<<" P ";
        }
        cout<<endl;
        for(int j=0;j<width;j++)
        {
            if(MAP[i][j].GetType()=='w') cout<<"H";
            if(MAP[i][j].GetType()=='h') cout<<"H";
            if(MAP[i][j].GetType()=='n') cout<<" ";
            if(MAP[i][j].GetType()=='d') cout<<" ";
            if(MAP[i][j].GetType()=='s') cout<<"   ";
            if(MAP[i][j].GetType()=='p') cout<<"   ";
            if(MAP[i][j].GetType()=='e') cout<<"   ";
            if(MAP[i][j].GetType()=='P') cout<<"   ";
        }
        cout<<endl;
    
        i++;
        for(int j=0;j<width;j++)
        {
            if(MAP[i][j].GetType()=='x') cout<<"H";
            if(MAP[i][j].GetType()=='w') cout<<"HHH";
            if(MAP[i][j].GetType()=='h') cout<<"HHH";
            if(MAP[i][j].GetType()=='n') cout<<"   ";
            if(MAP[i][j].GetType()=='d') cout<<"   ";
        }
        cout<<endl;
        i++;
    }
}

/*
void Map::TestMap(){

    height=21;
    width=21;

    MAP=new MazePart*[21];
    for(int i=0;i<21;i++)
    {
        MAP[i]=new MazePart[21];
    }

    for(int i=0;i<21;i++)
    {
        for(int j=0;j<21;j++)
        {
            if(IsDontCare(i,j)) MAP[i][j].SetType('x');
            if(IsPath(i,j)) MAP[i][j].SetType('p');
            if(IsWall(i,j)) MAP[i][j].SetType('w');
            if((i==0 || i==20) && IsWall(i,j)) MAP[i][j].SetType('h');
            if((j==0 || j==20) && IsWall(i,j)) MAP[i][j].SetType('h');
        }
    }

    string PATH="sssssdwdsdddwwwdddssd";
    int X=1, Y=1; 
    for(int i=0;i<PATH.size();i++)
    {
        if(i==0 || i==1)
        {
            if(MAP[Y-1][X].GetType()!='n') MAP[Y-1][X].SetType('h');
            if(MAP[Y+1][X].GetType()!='n') MAP[Y+1][X].SetType('h');
            if(MAP[Y][X+1].GetType()!='n') MAP[Y][X+1].SetType('h');
            if(MAP[Y][X-1].GetType()!='n') MAP[Y][X-1].SetType('h');
        }
        switch(PATH[i])
        {
            case 'w': 
            {
                Y--;
                MAP[Y][X].SetType('n');
                Y--;
                break;
            }
            case 'a':
            {
                X--;
                MAP[Y][X].SetType('n');
                X--;
                break;
            }
            case 's':
            {
                Y++;
                MAP[Y][X].SetType('n');
                Y++;
                break;
            }
            case 'd':
            {
                X++;
                MAP[Y][X].SetType('n');
                X++;
                break;
            }
        }

        if(i==PATH.size()-1) MAP[Y][X].SetType('e');
        if(i==PATH.size()-1 || i==PATH.size()-2)
        {
            if(MAP[Y-1][X].GetType()!='n') MAP[Y-1][X].SetType('h');
            if(MAP[Y+1][X].GetType()!='n') MAP[Y+1][X].SetType('h');
            if(MAP[Y][X+1].GetType()!='n') MAP[Y][X+1].SetType('h');
            if(MAP[Y][X-1].GetType()!='n') MAP[Y][X-1].SetType('h');
        }
    }
}*/

void Map::RefreshPlayer(){
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(IsPath(i,j) && MAP[i][j].GetType()=='P') MAP[i][j].SetType('p');
        }
    }
}

void Map::Play(){
    int X=1, Y=1, counter=0;
    MAP[Y][X].SetType('P');
    char c;
    while(Y!=height-2 || X!=width-2)
    {
        ShowMap();
        RefreshPlayer();
        cout<<"\nMove: "; cin>>c;
        switch(c)
        {
            default:
            {
                cout<<"\n!! Just type w a s d to move !!";
                break;
            }
            case 'w':
            {
                if(MAP[Y-1][X].GetType()=='w' || MAP[Y-1][X].GetType()=='h')
                {
                    cout<<"\n!! Invalid move !!";
                    break;
                }
                Y-=2;
                counter++;
                break;
            }
            case 'a':
            {
                if(MAP[Y][X-1].GetType()=='w' || MAP[Y][X-1].GetType()=='h')
                {
                    cout<<"\n!! Invalid move !!";
                    break;
                }
                X-=2;
                counter++;
                break;
            }
            case 's':
            {
                if(MAP[Y+1][X].GetType()=='w' || MAP[Y+1][X].GetType()=='h')
                {
                    cout<<"\n!! Invalid move !!";
                    break;
                }
                Y+=2;
                counter++;
                break;
            }
            case 'd':
            {
                if(MAP[Y][X+1].GetType()=='w' || MAP[Y][X+1].GetType()=='h')
                {
                    cout<<"\n!! Invalid move !!";
                    break;
                }
                X+=2;
                counter++;
                break;
            }
        }
        MAP[Y][X].SetType('P');
        cout<<"\n\n\n\n\n\n\n\n";
    }
    ShowMap();
    cout<<"\n\nFinished in "<<counter<<" moves.";
    if(counter==PATH.size()) cout<<"\nThat's the perfect game!!";
    cout<<"\n!!!!!!!!!\nCONGRATS\n!!!!!!!!!";
    return;
}    