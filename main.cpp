#include <iostream>
using namespace std;
#include "list"



// -- main funcs; ---------

void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void nextLine(int times = 1){
    for ( int i = 0 ; i < times; i++) cout << "\n";
}

void showMenu(){
    cout << "1. Сформировать дерево"; nextLine();
    cout << "2. Вывести дерево"; nextLine();
    cout << "3. Удалить дерево"; nextLine();
}

int randint(){
    return rand() % 200 - 100;
}


// -----------
struct trunk{
    string str;
    trunk* prev;

    trunk(trunk* prevAdr, string prev_str){
        str = prev_str;
        prev = prevAdr;
    }

};


void showTrunk( trunk* p, int &count){
    if( p == nullptr ) return;
    showTrunk(p->prev, count);
    count++;
    cout << p->str;
}


class Node{
public:
    int data;
    int height;
    Node *right, *left;

    explicit Node(int value){
        data = value;
        right = left = nullptr;
    }

};


class Tree{
public:
    Node *root = nullptr;

    void insert(int value){
        Node *node = new Node(value);
        if (!root){
            root = node;
        }else{
            Node *cur = root;
            while (true){
                if ( node->data < cur->data ){
                    if ( cur->left ){
                        cur = cur->left;
                    }else{
                        cur->left = node;
                        break;
                    }
                }else if ( cur->data == node->data ) {
                    if ( cur->right ) {
                        node->right = cur->right;
                    }
                    cur->right = node;
                    break;
                }else{
                    if ( cur->right ){
                        cur = cur->right;
                    }else{
                        cur->right = node;
                        break;
                    }
                }
            }
        }
    }



    void printTree(trunk* prev, bool isRight, Node *cur) {
        if ( !cur )
            return;
        string prev_str = "    ";//отступ по уровням (длина как для стрелки)
        trunk* tmp = new trunk(prev, prev_str);//новая связь
        printTree(tmp, true, cur->right);//правое поддерево
        if (!prev)//если нет предыдущего узла (предка) -> корень дерева
            tmp->str = "-->";//связь корня дерева
        else if (isRight) {//если правое поддерево
            tmp->str = ".-->";//связь правого поддерева
            prev_str = "   |";//в отступ по уровням добавляем черту (дерево идет вширь)
        }
        else {//в противном случае - левое поддерево
            tmp->str = "`-->";//левое поддерево
            prev->str = prev_str;//отступ по уровням не меняется
        }
        int count = 0;//уровень узла
        showTrunk(tmp, count);//выводим связи дерева - стебли
        cout << cur->data << endl;//выводим значение узла
        if (prev)//задаем строку отступов для узла, если есть поддеревья
            prev->str = prev_str;
        tmp->str = "   |";//в отступ по уровням добавляем черту (дерево идет вширь)
        printTree(tmp, 0, cur->left);//левое поддерево
    }

    void destroyTree(Node *root)
    {
        if( root )	    	        // если узел дерева существует
        {					// функция вызовет сама себя
            destroyTree(root->left);	// сначала для левого потомка,
            destroyTree(root->right);  	// после для правого потомка.
            delete root;			// если потомков нет, она удалит
        }
    }

};


int main() {
    char choice;
    int num, len;
    list<int> numbers;
    Tree *tree = new Tree;

    for ( int i = 0; i < 100; i++){
        showMenu(); nextLine();
        cout << "Выберите: "; cin >> choice;

        switch (choice) {
            case '1':

                cout << "a: Самостоятельно ввести."; nextLine();
                cout << "b: Автоматическое заполнение."; nextLine();
                cout << "Выбор за вами: "; cin >> choice; clearStream();
                if ( choice == 'a'){
                    while ( cin >> num ){
                        numbers.push_back(num);
                        if(cin.peek() == '\n'){
                            break;
                        }
                    }
                }else{
                    cout << "Введите длину: "; cin >> len; clearStream();
                    for ( int i = 0 ; i < len; i++ ){
                        numbers.push_back(randint());
                    }
                }

                for ( auto iter = numbers.begin(); iter != numbers.end(); iter++ ){
                    tree->insert(*iter);
                }
                numbers.clear();

                break;

            case '2':
                tree->printTree(nullptr, true, tree->root);
                break;

            case '3':
                tree->destroyTree(tree->root);
                cout << "Дерево уничтожено"; nextLine();
                break;

            default:
                cout << "error";
                nextLine();
        }


    }


}
