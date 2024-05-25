#include <iostream>
#include "list"
#include "ctime"
#include <fstream>
#include "string"

using namespace std;
using namespace chrono;
using time_mngr = time_point<steady_clock, duration<__int64_t, ratio<1, 1000000000>>>;

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
    cout << "3. Операции над деревом"; nextLine();
    cout << "4. Скорость проверки на сбалансированность"; nextLine();
    cout << "5. Варианты"; nextLine();

}


int randint(){
    return rand() % 200 - 100;
}

// ---- trunks -------------
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


struct trunkStream {
    string str;
    trunkStream *prev;

    trunkStream(trunkStream *prevAdr, string prev_str){
        str = prev_str;
        prev = prevAdr;
    }

};

void showTrunkStream(trunkStream *p, int &count, ofstream& f2) {
    if (p == nullptr) return;
    showTrunkStream(p->prev, count, f2);
    count++;
    f2 << p->str;
}

// AVL-tree -------------

class Node{
private:
    int data;
    int height;
    Node *right, *left;

public:
    explicit Node(int value){
        data = value;
        right = left = nullptr;
    }

    friend class Tree;
};


class Tree{

public:
    Node *root = nullptr;


    bool isBalanced( Node* node ){
        if ( !node ){
            return true;
        }
        if ( abs(node->right - node->left) >= 2 ){
            return false;
        }
        return isBalanced(node->left) && isBalanced(node->right);

    }

    Node *insert(int value, Node *node){

        if ( !node ){
            return new Node(value);
        }
        else if ( value > node->data ){
            node->right = insert(value, node->right);
        }else{
            node->left = insert(value, node->left);
        }
        return balance(node);
    }


    void print(trunk* prev, bool isRight, Node *cur) {
        if ( !cur )
            return;
        string prev_str = "    ";
        trunk* tmp = new trunk(prev, prev_str);
        print(tmp, true, cur->right);
        if (!prev)
            tmp->str = "-->";
        else if (isRight) {
            tmp->str = ",-->";
            prev_str = "   |";
        }
        else {
            tmp->str = "`-->";
            prev->str = prev_str;
        }
        int count = 0;//
        showTrunk(tmp, count);
        cout << cur->data << "(" << cur->height << ")" << endl;
        if (prev)
            prev->str = prev_str;
        tmp->str = "   |";
        print(tmp, false, cur->left);
    }



    int getHeight(Node *cur){
        if (!cur){
            return 0;
        }
        cur->height = max(getHeight(cur->left), getHeight(cur->right)) + 1;
        return cur->height;
    }


    Node *search(int value, Node* node){
        if ( !node ){
            return nullptr;
        }
        if ( node->data == value ){
            return node;
        }
        if ( value > node->data ){
            return search(value, node->right);
        }else{
            return search(value, node->left);
        }
    }


    Node *findMax(Node *node){
        if ( !node->right ) return node;
        else{
            return findMax(node->right);
        }
    }


    Node *deleteNode(int value, Node *node){
        if ( !node ) return nullptr;
        if ( value > node->data ) node->right =  deleteNode(value, node->right);
        else if ( value < node->data) node->left = deleteNode(value, node->left);
        else{
            if ( !node->right || !node->left ) {

                if (!node->left && !node->right) {
                    delete node;
                    return nullptr;
                } else if (!node->left) {
                    Node *tmp = node->right;
                    delete node;
                    return balance(tmp);
                } else if (!node->right) {
                    Node *tmp = node->left;
                    delete node;
                    return balance(tmp);
                }
            }else {
                Node *tmp = findMax(node->left);
                node->data = tmp->data;
                node->left = deleteNode(node->data, node->left);
                return balance(node);
            }
        }
        if( node ){
            getHeight(node);
        }
        return balance(node);
    }


    Node *leftRotate(Node *cur){
        Node *r = cur->right;
        cur->right = r->left;
        r->left = cur;
        getHeight(cur);
        getHeight(r);
        return r;
    }


    Node *rightRotate(Node *cur){
        Node *r = cur->left;
        cur->left = r->right;
        r->right = cur;
        getHeight(cur);
        getHeight(r);
        return r;
    }


    Node *balance(Node *cur){
        getHeight(cur);
            if ( getHeight(cur->right) - getHeight(cur->left ) == 2 ){
                if ( getHeight(cur->right->left) > getHeight(cur->right->right) ){
                    cur->right = rightRotate(cur->right);
                }
                return leftRotate(cur);
            }else if ( getHeight(cur->right) - getHeight(cur->left) == -2 ){
                if ( getHeight(cur->left->left) < getHeight(cur->left->right) ){
                    cur->left = leftRotate(cur->left);
                }
                return rightRotate(cur);
            }

        return cur;
    }


    void printFile(trunkStream* prev, bool isRight, Node *cur, ofstream& f2) {
        if ( !cur )
            return;
        string prev_str = "    ";
        trunkStream* tmp = new trunkStream(prev, prev_str);
        printFile(tmp, true, cur->right, f2);
        if (!prev)
            tmp->str = "-->";
        else if (isRight) {
            tmp->str = ",-->";
            prev_str = "   |";
        }
        else {
            tmp->str = "`-->";
            prev->str = prev_str;
        }
        int count = 0;//
        showTrunkStream(tmp, count, f2);
        f2 << cur->data << "(" << cur->height << ")" << endl;
        if (prev)
            prev->str = prev_str;
        tmp->str = "   |";
        printFile(tmp, false, cur->left, f2);
    }

};



int main() {
    time_mngr start, end;
    nanoseconds result;
    int choice, num, len;
    Node *res;
    list<int> numbers;
    Tree *tree = new Tree;
    bool balance;
    int variants;
    string problem;

    srand(time(NULL));


    for ( int i = 0; i < 100; i++){
        showMenu(); nextLine();
        cout << "Выберите: "; cin >> choice;

        switch (choice) {
            case 1:
                cout << "1: Самостоятельно ввести."; nextLine();
                cout << "2: Автоматическое заполнение."; nextLine();
                cout << "Выбор за вами: "; cin >> choice; clearStream();

                if ( choice == 1){
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
                start = steady_clock::now();
                for ( auto iter = numbers.begin(); iter != numbers.end(); iter++ ){
                    tree->root = tree->insert(*iter, tree->root);
                }
                tree->getHeight(tree->root);
                end = steady_clock::now();
                result = end - start;
                cout << "АВЛ-дерево сформировано за " << result.count() / 1000 << " мс."; nextLine(2);
                numbers.clear();

                break;

            case 2:
                tree->print(nullptr, true, tree->root);
                nextLine();
                break;

            case 3:
                cout << "1. Вставка элемента:"; nextLine();
                cout << "2. удаление элемента:"; nextLine();
                cout << "3. получение элемента:"; nextLine();
                cout << "Выбор за вами: "; cin >> choice; clearStream();
                cout << "Элемент: "; cin >> num; clearStream();
                switch (choice) {
                    case 1:
                        start = steady_clock::now();
                        tree->root = tree->insert(num, tree->root);
                        end = steady_clock::now();
                        cout << "Вставка прошла за " << (end - start).count() << "нс";
                        nextLine();
                        break;

                    case 2:
                        start = steady_clock::now();
                        tree->root = tree->deleteNode(num, tree->root);
                        end = steady_clock::now();
                        cout << "Удаление прошло за " << (end - start).count() << "нс";
                        nextLine();
                        break;

                    case 3:
                        start = steady_clock::now();
                        res = tree->search(num, tree->root);
                        end = steady_clock::now();
                        cout << "Элемент находится по адресу " << res;
                        nextLine();
                        cout << "Поиск прошел за " << (end - start).count() << "нс";
                        nextLine();
                        break;
                }

            case 4:
                start = steady_clock::now();
                balance = tree->isBalanced(tree->root);
                end = steady_clock::now();
                cout << "Дерево " << (balance ? "не" : "") << " сбалансированно";
                nextLine();
                cout << "Проверка прошла за " << (end - start).count() / 1000 << " мс";
                nextLine();
                break;

            case 5:
                cout << "Кол-во вариантов: "; cin >> variants; clearStream();
                vector<int> data;
                int insertItem = randint();

                ofstream variantOut;
                ofstream keyOut;
                ofstream answerOut;

                variantOut.open("/Users/kirillbelaev/CLionProjects/AVL-tree/variant.txt");
                keyOut.open("/Users/kirillbelaev/CLionProjects/AVL-tree/key.txt");
                answerOut.open("/Users/kirillbelaev/CLionProjects/AVL-tree/answer.txt");
                for ( int variant = 1; variant <= variants; variant++ ) {
                    Tree *printTree = new Tree;
                    data.clear();

                    problem = "ВАРИАНТ " + to_string(variant) + "\n";
                    answerOut << "ВАРИАНТ " << to_string(variant) << ":\n\n";
                    keyOut << "ВАРИАНТ " << to_string(variant) << ":\n";

                    for (int j = 0; j < 10; j++) data.push_back(randint());

                    problem += "1. Создать АВЛ-дерево из элементов: ";
                    answerOut << "1. Создать АВЛ-дерево из элементов: ";
                    keyOut << "1. Создать АВЛ-дерево из элементов: ";

                    problem += to_string(data[0]);
                    answerOut << to_string(data[0]);
                    keyOut << to_string(data[0]);
                    for (int j = 1; j < 10; j++){
                        problem += (", " + to_string(data[j]));
                        answerOut << ", " + to_string(data[j]);
                        keyOut << ", " + to_string(data[j]);
                    }
                    problem += "\n";
                    answerOut << "\n\n";
                    keyOut << "\n";
                    for (int j = 0; j < 10; j++){
                        printTree->root = printTree->insert(data[j], printTree->root);
                        printTree->printFile(nullptr, true, printTree->root, answerOut);
                        answerOut << "\n-------------------------------------------------------\n\n";
                    }

                    problem += "2. Вставить элемент " + to_string(insertItem) + " в дерево\n";
                    answerOut << "2. Вставить элемент " + to_string(insertItem) + " в дерево\n\n";
                    keyOut << "2. Вставить элемент " + to_string(insertItem) + " в дерево\n";

                    printTree->root = printTree->insert(insertItem, printTree->root);
                    printTree->printFile(nullptr, true, printTree->root, answerOut);
                    answerOut << "\n-------------------------------------------------------\n\n";

                    int deleteElem = rand() % 10;
                    problem += "3. Удалить элемент " + to_string(data[deleteElem]) + " из дерева\n\n";
                    answerOut << "3. Удалить элемент " + to_string(data[deleteElem]) + " из дерева\n\n";
                    keyOut << "3. Удалить элемент " + to_string(data[deleteElem]) + " из дерева\n\n";

                    printTree->root = printTree->deleteNode(data[deleteElem], printTree->root);
                    printTree->printFile(nullptr, true, printTree->root, answerOut);
                    answerOut << "\n\n";
                    keyOut << "\n";
                    variantOut << problem;
                    printTree->printFile(nullptr, true, printTree->root, keyOut);

                }

                variantOut.close();
                answerOut.close();
                keyOut.close();

                break;


//                default:
//                    cout << "error";
//                    nextLine();

        }
    }
}

