#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <cstdio>
#include <chrono>
#include <random>
#include <map>
#include <thread>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std;

void pause_time(){
    usleep(500000);
}

/*void pause_time() {
    std::this_thread::sleep_for(std::chrono::microseconds(500000));
}*/

typedef struct player {
    string player_name;
    vector<int> poker_on_hand;
    struct player* pre;
    struct player* next;
} Player;

class Game {
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
    Game() {};

    void bubbleSort(vector<int>& arr, vector<int>& arr2, int n) {
        int i, j;
        for (i = 0; i < n - 1; i++)
            for (j = 0; j < n - i - 1; j++)
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                    swap(arr2[j], arr2[j + 1]);
                }
    }

    void generate_poker() {
        for (int i = 0; i < 52; i++) this->poker_tmp.push_back(i);
    }

    void Initialize() {
        Player* tmpB = new Player();
        Player* tmpC = new Player();
        Player* tmpD = new Player();

        this->B = tmpB;
        this->C = tmpC;
        this->D = tmpD;
        this->user->pre = this->D;
        this->user->next = this->B;
        this->B->pre = this->user;
        this->B->next = this->C;
        this->B->player_name = "���aB";
        this->C->pre = this->B;
        this->C->next = this->D;
        this->C->player_name = "���aC";
        this->D->pre = this->C;
        this->D->next = this->user;
        this->D->player_name = "���aD";

        this->current_player_num = 4;
        this->current_num = 0;
        player_in = true;
        generate_poker(); //�ͦ�52�i���J�P
    }

    void wash() {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(this->poker_tmp.begin(), this->poker_tmp.end(), default_random_engine(seed));
        for (int i = 0; i < this->poker_tmp.size(); i++) this->poker_add.push(this->poker_tmp[i]);
        this->poker_tmp.clear();
    }

    void deal() {
        int full = 0;
        Player* tmp = this->current_player;
        while (full < this->current_player_num) {
            if (this->poker_add.empty()) wash();
            if (tmp->poker_on_hand.size() < 5) {
                int top_poker = this->poker_add.top();
                tmp->poker_on_hand.push_back(top_poker);
                this->poker_add.pop();
            }
            else {
                full++;
            }
            tmp = tmp->next;
        }
    }

    string get_poker(int n) {
        string flower, num;
        switch (n / 13) {
        case 0:
            flower = "�®�"; //�®�
            break;
        case 1:
            flower = "����"; //����
            break;
        case 2:
            flower = "���"; //���
            break;
        case 3:
            flower = "����"; //����
            break;
        }
        switch (n % 13) {
        case 0:
            num = "K";
            break;
        case 1:
            num = "A";
            break;
        case 11:
            num = "J";
            break;
        case 12:
            num = "Q";
            break;
        default:
            num = to_string(n % 13);
            break;
        }
        return flower + " " + num;
    }

    void show_player_card() {
        cout << "�A����P�p�U�A�п�J�n�X����PID:" << endl;
        cout << "ID\t��P" << endl;
        for (int i = 0; i < this->current_player->poker_on_hand.size(); i++) {
            string real_poker = get_poker(this->current_player->poker_on_hand[i]); //���o��ڼ��J�P���
            cout << i << "\t" << real_poker << endl;
        }
    }

    void reverse_player() {
        pause_time();
        cout << "���Ǥ���!" << endl;
        Player* tmp = new Player();
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

    void assign_player() {
        int assign_input;
        map<string, Player*> menu;
        vector<string> menu_list;
        map<string, Player*>::iterator it;
        Player* current = new Player();
        current = this->current_player->next;

        pause_time();
        cout << "�A�i�H���w�����a!!" << endl;
        cout << "ID\t���a�W��" << endl;
        int counter = 0;
        while (current != this->current_player) {
            menu[current->player_name] = current;
            cout << counter << "\t" << current->player_name << endl;
            counter++;
            menu_list.push_back(current->player_name);
            current = current->next;
        }

        if (this->current_player == this->user) {
            pause_time();
            cout << "�п�JID�H���w���a: ";
            while (cin >> assign_input) {
                pause_time();
                if (assign_input >= 0 & assign_input < menu_list.size()) break;
                else {
                    cout << "�п�J���T��ID!!" << endl;
                    pause_time();
                    cout << "�A�n���w�����aID: ";
                }
            }
            it = menu.find(menu_list[assign_input]);
        }
        else {
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            shuffle(menu_list.begin(), menu_list.end(), default_random_engine(seed));
            it = menu.find(menu_list[0]);
        }
        pause_time();
        cout << this->current_player->player_name << "���w" << it->second->player_name << endl;
        this->current_player = it->second;
    }

    void process(int card) {
        if (card == 1) this->current_num = 0;
        else {
            switch (card % 13) {
            case 0:
                this->current_num = 99;
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
                this->current_num = this->current_num;
                break;
            case 10:
                if (this->current_num > 89) this->current_num -= 10;
                else this->current_num += 10;
                break;
            case 12:
                if (this->current_num > 79) this->current_num -= 20;
                else this->current_num += 20;
                break;
            default:
                this->current_num += card % 13;
                break;
            }
        }
    }

    void Real_user() {
        cout << "����A�F!" << endl;
        pause_time();
        cout << "�{�b���ƭȬ�: " << this->current_num << endl;
        pause_time();
        show_player_card();
        string user_input0;
        cout << "�A�n�X��PID: ";
        while (cin >> user_input0) {
            pause_time();
            if (user_input0 == "0" | user_input0 == "1" | user_input0 == "2" | user_input0 == "3" | user_input0 == "4") break;
            else {
                cout << "�п�J���T��ID!" << endl;
                pause_time();
                cout << "�A�n�X���P: ";
            }
        }
        int user_input = stoi(user_input0);
        int user_out_poker = this->user->poker_on_hand[user_input];
        string real_poker = get_poker(user_out_poker);

        this->poker_tmp.push_back(user_out_poker); //�⥴�X���P��Jtmp�P��
        //���X����P���X
        if (user_input != 4) swap(this->user->poker_on_hand[user_input], this->user->poker_on_hand[4]);
        this->user->poker_on_hand.pop_back();

        Player* tmp = new Player();
        tmp = this->current_player;
        process(user_out_poker);
        pause_time();
        cout << tmp->player_name << "���X" << real_poker << "�A�ƭ��ܬ�" << this->current_num << endl;
        if (this->current_num > 99) {
            pause_time();
            cout << "�{�b���ƭȬ�: " << this->current_num << "�A�w�g�j��99!" << endl;
            pause_time();
            cout << "�A��F!!" << endl;
            this->player_in = false;
        }
        else {
            deal();
            if (tmp == this->current_player) this->current_player = this->current_player->next;
        }
    }

    void Drop_user(Player* loss) {
        for (int i = 0; i < loss->poker_on_hand.size(); i++) this->poker_tmp.push_back(loss->poker_on_hand[i]);
        loss->next->pre = loss->pre;
        loss->pre->next = loss->next;
        loss->next = NULL;
        loss->pre = NULL;
    }

    void Robot() {
        vector<int> normal_card_list;
        vector<int> normal_ID_list;
        vector<int> save_card_list;
        vector<int> save_num_list;
        int coda = 99 - this->current_num;
        int tmp_num = this->current_num;
        cout << "����" << this->current_player->player_name << "�F!" << endl;
        pause_time();
        cout << "�{�b���ƭȬ�: " << this->current_num << endl;

        for (int i = 0; i < 5; i++) {
            int current_card = this->current_player->poker_on_hand[i];
            if (current_card != 1 & current_card % 13 != 0 & current_card % 13 != 4 & current_card % 13 != 5 & current_card % 13 != 10 & current_card % 13 != 11 & current_card % 13 != 12) {
                normal_card_list.push_back(current_card % 13);
                normal_ID_list.push_back(i);
            }
            else {
                save_card_list.push_back(i);
                save_num_list.push_back(current_card % 13);
            }
        }

        if (normal_card_list.size() > 1) bubbleSort(normal_card_list, normal_ID_list, normal_card_list.size());

        if (normal_card_list.size() == 0) {
            bubbleSort(save_num_list, save_card_list, normal_card_list.size());
            int robot_out_poker = this->current_player->poker_on_hand[save_card_list[0]];
            this->poker_tmp.push_back(robot_out_poker); //�⥴�X���P��Jtmp�P��
            string real_poker = get_poker(robot_out_poker);

            //���X����P���X
            if (save_card_list[0] != 4) swap(this->current_player->poker_on_hand[save_card_list[0]], this->current_player->poker_on_hand[4]);
            this->current_player->poker_on_hand.pop_back();
            Player* tmp = new Player();
            tmp = this->current_player;
            process(robot_out_poker);
            pause_time();
            cout << tmp->player_name << "���X" << real_poker << "�A�ƭ��ܬ�" << this->current_num << endl;
            deal();
            if (tmp == this->current_player) this->current_player = this->current_player->next;
        }
        else if (coda < normal_card_list[0]) {
            if (save_card_list.size() == 0) {
                pause_time();
                cout << this->current_player->player_name << "��F!" << endl;
                this->current_player_num--;
                this->current_player = this->current_player->next;
                Drop_user(this->current_player->pre);
            }
            else {
                int robot_out_poker = this->current_player->poker_on_hand[save_card_list[0]];
                this->poker_tmp.push_back(robot_out_poker); //�⥴�X���P��Jtmp�P��
                string real_poker = get_poker(robot_out_poker);

                //���X����P���X
                if (save_card_list[0] != 4) swap(this->current_player->poker_on_hand[save_card_list[0]], this->current_player->poker_on_hand[4]);
                this->current_player->poker_on_hand.pop_back();
                Player* tmp = new Player();
                tmp = this->current_player;
                process(robot_out_poker);
                pause_time();
                cout << tmp->player_name << "���X" << real_poker << "�A�ƭ��ܬ�" << this->current_num << endl;
                deal();
                if (tmp == this->current_player) this->current_player = this->current_player->next;
            }
        }
        else {
            int max_card_id;
            for (int ma = 0; ma < normal_card_list.size(); ma++) {
                if (normal_card_list[ma] > coda) {
                    max_card_id = normal_ID_list[ma - 1];
                    break;
                }
                else {
                    max_card_id = normal_ID_list[ma];
                }
            }
            int robot_out_poker = this->current_player->poker_on_hand[max_card_id];

            string real_poker = get_poker(robot_out_poker);
            this->poker_tmp.push_back(robot_out_poker); //�⥴�X���P��Jtmp�P��
            //���X����P���X
            if (max_card_id != 4) swap(this->current_player->poker_on_hand[max_card_id], this->current_player->poker_on_hand[4]);
            this->current_player->poker_on_hand.pop_back();
            process(robot_out_poker);
            pause_time();
            cout << this->current_player->player_name << "���X" << real_poker << "�A�ƭ��ܬ�" << this->current_num << endl;
            deal();
            this->current_player = this->current_player->next;
        }
        normal_card_list.clear();
        normal_ID_list.clear();
        save_card_list.clear();
    }

    void Game_start() {
        string name;
        cout << "�п�J���a�W��: ";
        while (getline(cin, name)) {
            if (name == "���aB" | name == "���aC" | name == "���aD")
                cout << "�P�q���W�٭��ơA�п�J�O���W��: ";
            else break;
        }

        Player* tmp = new Player();
        tmp->player_name = name;
        this->user = tmp;
        this->current_player = this->user;

        cout << "�C���}�l!!" << endl;
        Initialize(); //��l�ƪ��a���ǻP�ͦ����J�P
        wash(); //�~�P
        deal(); //�o�P
        pause_time();

        while (this->current_player_num > 1 & this->player_in) {
            cout << endl;
            if (this->current_player == this->user) {
                Real_user();
                pause_time();
            }
            else {
                Robot();
                pause_time();
            }

        }
        pause_time();
        if (this->current_player_num == 1) cout << "�AĹ�F!!" << endl;
        pause_time();
        cout << "�P�±z���C��!!" << endl;
    }
};

int main() {
    Game Game_99;

    Game_99.Game_start();

    return 0;
}
