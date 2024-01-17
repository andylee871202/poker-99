#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <chrono>
#include <random>
#include <map>
#include <thread>
#include <unistd.h>

using namespace std;

void pause_time(){
    usleep(500000);
}

typedef struct player{
    string player_name;
    vector<int> poker_on_hand;
    struct player* pre;
    struct player* next;
} Player;

class Game{
private:
    Player* user;
    Player* B;
    Player* C;
    Player* D;
    Player* current_player;
    int current_num;
    int current_player_num;
    bool player_in;
    stack<int> poker_add;
    vector<int> poker_tmp;
    vector<int> save_card;

public:
    Game (){};

    void bubbleSort(vector<int> &arr,vector<int> &arr2, int n){
        int i, j;
        for (i = 0; i < n - 1; i++)
            for (j = 0; j < n - i - 1; j++)
                if (arr[j] > arr[j + 1]){
                    swap(arr[j], arr[j + 1]);
                    swap(arr2[j], arr2[j + 1]);
                }
    }

    void generate_poker(){
        for (int i=0;i<52;i++){
            this->poker_tmp.push_back(i);
            if (i==1) this->save_card.push_back(i);
            else{
                switch (i%13){
                case 0:
                    this->save_card.push_back(i);
                    break;
                case 4:
                    this->save_card.push_back(i);
                    break;
                case 5:
                    this->save_card.push_back(i);
                    break;
                case 11:
                    this->save_card.push_back(i);
                    break;
                case 10:
                    this->save_card.push_back(i);
                    break;
                case 12:
                    this->save_card.push_back(i);
                    break;
                }
            }
        }
    }

    void Initialize(){
        Player *tmpB = new Player();
        Player *tmpC = new Player();
        Player *tmpD = new Player();

        this->B = tmpB;
        this->C = tmpC;
        this->D = tmpD;
        this->user->pre = this->D;
        this->user->next = this->B;
        this->B->pre = this->user;
        this->B->next = this->C;
        this->B->player_name = "Player B";
        this->C->pre = this->B;
        this->C->next = this->D;
        this->C->player_name = "Player C";
        this->D->pre = this->C;
        this->D->next = this->user;
        this->D->player_name = "Player D";

        this->current_player_num=4;
        this->current_num=0;
        player_in=true;
        generate_poker(); //生成52張撲克牌
    }

    void wash(){
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(this->poker_tmp.begin(), this->poker_tmp.end(), default_random_engine(seed));
        for (int i=0;i<this->poker_tmp.size();i++) this->poker_add.push(this->poker_tmp[i]);
        this->poker_tmp.clear();
    }

    void deal(){
        int full=0;
        Player* tmp=this->current_player;
        while (full<this->current_player_num){
            if (this->poker_add.empty()) wash();
            if (tmp->poker_on_hand.size()<5){
                int top_poker = this->poker_add.top();
                tmp->poker_on_hand.push_back(top_poker);
                this->poker_add.pop();
            }else{
                full++;
            }
            tmp = tmp->next;
        }
    }

    string get_poker(int n){
        string flower, num;
        switch (n/13){
        case 0:
            flower="Spade"; //黑桃
            break;
        case 1:
            flower="Heart"; //紅心
            break;
        case 2:
            flower="Diamond"; //方塊
            break;
        case 3:
            flower="Club"; //梅花
            break;
        }
        switch (n%13){
        case 0:
            num="K";
            break;
        case 1:
            num="A";
            break;
        case 11:
            num="J";
            break;
        case 12:
            num="Q";
            break;
        default:
            num=to_string(n%13);
            break;
        }
        return flower+" "+num;
    }

    void show_player_card(){
        cout << "Your hands are showed below, please input ID to play:" << endl;
        cout << "ID\tHand" << endl;
        for (int i=0;i<this->current_player->poker_on_hand.size();i++){
            string real_poker = get_poker(this->current_player->poker_on_hand[i]); //取得實際撲克牌花色
            cout << i << "\t" << real_poker << endl;
        }
    }

    void reverse_player(){
        pause_time();
        cout << "Reverse!" << endl;
        Player *tmp = new Player();
        tmp = this->user->next;
        this->user->next = this->user->pre;
        this->user->pre = tmp;

        tmp = this->B->next;
        this->B->next = this->B->pre;
        this->B->pre = tmp;

        tmp = this->C->next;
        this->C->next = this->C->pre;
        this->C->pre = tmp;

        tmp = this->D->next;
        this->D->next = this->D->pre;
        this->D->pre = tmp;
    }

    void assign_player(){
        int assign_input;
        map<string, Player*> menu;
        vector<string> menu_list;
        map<string, Player*>::iterator it;
        Player *current = new Player();
        current = this->current_player->next;

        pause_time();
        cout << "Player you can assign!!" << endl;
        cout << "ID\tplayer" << endl;
        int counter=0;
        while (current!=this->current_player){
            menu[current->player_name] = current;
            cout << counter << "\t" << current->player_name << endl;
            counter++;
            menu_list.push_back(current->player_name);
            current = current->next;
        }

        if (this->current_player == this->user){
            pause_time();
            cout << "Please input ID to assign: ";
            while (cin >> assign_input){
                pause_time();
                if (assign_input>=0 & assign_input<=menu_list.size()) break;
                else{
                    cout << "Please input correct ID!!" << endl;
                    pause_time();
                    cout << "Your input: ";
                }
            }
            it = menu.find(menu_list[assign_input]);
        }else{
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            shuffle(menu_list.begin(), menu_list.end(), default_random_engine(seed));
            it = menu.find(menu_list[0]);
        }
        pause_time();
        cout << this->current_player->player_name << " assign to " << it->second->player_name << endl;
        this->current_player = it->second;
    }

    void process(int card){
        if (card==1) this->current_num=0;
        else{
            switch (card%13){
            case 0:
                this->current_num=99;
                break;
            case 4:
                this->current_num = this->current_num;
                reverse_player();
                break;
            case 5:
                this->current_num = this->current_num;
                assign_player();
                break;
            case 11:
                this->current_num=this->current_num;
                break;
            case 10:
                if (this->current_num>89) this->current_num-=10;
                else this->current_num+=10;
                break;
            case 12:
                if (this->current_num>79) this->current_num-=20;
                else this->current_num+=20;
                break;
            default:
                this->current_num+=card%13;
                break;
            }
        }
    }

    void Real_user(){
        cout << "Your turn!" << endl;
        pause_time();
        cout << "Current value : " << this->current_num << endl;
        pause_time();
        show_player_card();
        string user_input0;
        cout << "Your input: ";
        while(cin >> user_input0){
            pause_time();
            if (user_input0=="0" | user_input0=="1" | user_input0=="2" | user_input0=="3" | user_input0=="4") break;
            else{
                cout << "Please input correct ID!" << endl;
                pause_time();
                cout << "Your input: ";
            }
        }
        int user_input = stoi(user_input0);
        int user_out_poker = this->user->poker_on_hand[user_input];
        string real_poker = get_poker(user_out_poker);

        this->poker_tmp.push_back(user_out_poker); //把打出的牌放入tmp牌堆
        //把丟出的手牌移出
        if (user_input!=4) swap(this->user->poker_on_hand[user_input],this->user->poker_on_hand[4]);
        this->user->poker_on_hand.pop_back();

        Player* tmp = new Player();
        tmp = this->current_player;
        process(user_out_poker);
        pause_time();
        cout << tmp->player_name << " played " << real_poker << ", turned value to "<< this->current_num << endl;
        if (this->current_num>99){
            pause_time();
            cout << "Current value : " << this->current_num << ", larger than 99!" << endl;
            pause_time();
            cout << "You lose!!" << endl;
            this->player_in = false;
        }else{
            deal();
            if (tmp==this->current_player) this->current_player = this->current_player->next;
        }
    }

    void Drop_user(Player* loss){
        for (int i=0;i<loss->poker_on_hand.size();i++) this->poker_tmp.push_back(loss->poker_on_hand[i]);
        loss->next->pre = loss->pre;
        loss->pre->next = loss->next;
        loss->next = NULL;
        loss->pre = NULL;
    }

    void Robot(){
        vector<int> normal_card_list;
        vector<int> normal_ID_list;
        vector<int> save_card_list;
        vector<int> save_num_list;
        int coda = 99-this->current_num;
        int tmp_num = this->current_num;
        cout << this->current_player->player_name <<" turn!" << endl;
        pause_time();
        cout << "Current value : " << this->current_num << endl;

        for (int i=0;i<5;i++){
            int current_card=this->current_player->poker_on_hand[i];
            if (current_card!=1 & current_card%13!=0  & current_card%13!=4  & current_card%13!=5 & current_card%13!=10 & current_card%13!=11 & current_card%13!=12){
                normal_card_list.push_back(current_card%13);
                normal_ID_list.push_back(i);
            }else{
                save_card_list.push_back(i);
                save_num_list.push_back(current_card%13);
            }
        }

        if (normal_card_list.size()>1) bubbleSort(normal_card_list,normal_ID_list,normal_card_list.size());

        if (normal_card_list.size()==0){
            bubbleSort(save_num_list,save_card_list,normal_card_list.size());
            int robot_out_poker = this->current_player->poker_on_hand[save_card_list[0]];
            this->poker_tmp.push_back(robot_out_poker); //把打出的牌放入tmp牌堆
            string real_poker = get_poker(robot_out_poker);

            //把丟出的手牌移出
            if (save_card_list[0]!=4) swap(this->current_player->poker_on_hand[save_card_list[0]],this->current_player->poker_on_hand[4]);
            this->current_player->poker_on_hand.pop_back();
            Player* tmp = new Player();
            tmp = this->current_player;
            process(robot_out_poker);
            pause_time();
            cout << tmp->player_name << " played " << real_poker << ", turned value to "<< this->current_num << endl;
            deal();
            if (tmp==this->current_player) this->current_player = this->current_player->next;
        }else if (coda<normal_card_list[0]){
            if (save_card_list.size()==0){
                pause_time();
                cout << this->current_player->player_name << " loss!" << endl;
                this->current_player_num--;
                this->current_player = this->current_player->next;
                Drop_user(this->current_player->pre);
            }else{
                int robot_out_poker = this->current_player->poker_on_hand[save_card_list[0]];
                this->poker_tmp.push_back(robot_out_poker); //把打出的牌放入tmp牌堆
                string real_poker = get_poker(robot_out_poker);

                //把丟出的手牌移出
                if (save_card_list[0]!=4) swap(this->current_player->poker_on_hand[save_card_list[0]],this->current_player->poker_on_hand[4]);
                this->current_player->poker_on_hand.pop_back();
                Player* tmp = new Player();
                tmp = this->current_player;
                process(robot_out_poker);
                pause_time();
                cout << tmp->player_name << " played " << real_poker << ", turned value to "<< this->current_num << endl;
                deal();
                if (tmp==this->current_player) this->current_player = this->current_player->next;
            }
        }else{
            int max_card_id;
            for (int ma=0;ma<normal_card_list.size();ma++){
                if (normal_card_list[ma]>coda){
                    max_card_id = normal_ID_list[ma-1];
                    break;
                }else{
                    max_card_id = normal_ID_list[ma];
                }
            }
            int robot_out_poker = this->current_player->poker_on_hand[max_card_id];

            string real_poker = get_poker(robot_out_poker);
            this->poker_tmp.push_back(robot_out_poker); //把打出的牌放入tmp牌堆
            //把丟出的手牌移出
            if (max_card_id!=4) swap(this->current_player->poker_on_hand[max_card_id],this->current_player->poker_on_hand[4]);
            this->current_player->poker_on_hand.pop_back();
            process(robot_out_poker);
            pause_time();
            cout << this->current_player->player_name << " played " << real_poker << ", turned value to "<< this->current_num << endl;
            deal();
            this->current_player = this->current_player->next;
        }
        normal_card_list.clear();
        normal_ID_list.clear();
        save_card_list.clear();
    }

    void Game_start(){
        string name;
        cout << "Please enter your name: ";
        while (getline(cin, name)){
            if (name=="Player B" | name=="Player C" | name=="Player D")
                cout << "Please enter another name: ";
            else break;
        }

        Player *tmp = new Player();
        tmp->player_name = name;
        this->user = tmp;
        this->current_player = this->user;

        cout << "Game Start!!" << endl;
        Initialize(); //初始化玩家順序與生成撲克牌
        wash(); //洗牌
        deal(); //發牌
        pause_time();

        while (this->current_player_num>1 & this->player_in){
            cout << endl;
            if (this->current_player == this->user){
                Real_user();
                pause_time();
            }else{
                Robot();
                pause_time();
            }

        }
        pause_time();
        if (this->current_player_num==1) cout << "You win!!" << endl;
        pause_time();
        cout << "Thanks for playing!!" << endl;
    }
};

int main(){
    Game Game_99;

    Game_99.Game_start();

    return 0;
}
