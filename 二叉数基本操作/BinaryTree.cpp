#include <iostream>
#include <queue>
#include <stack>
using namespace std;

template <class Type>
class BinaryTree{
private:
    struct Node{   //二叉树的结点类
       Node *left,*right;  //结点的左右儿子的地址
       Type data;  //结点的数据信息

       //构造函数和析构函数
       Node():left(NULL),right(NULL){}
       Node(Type item,Node *L=NULL,Node *R=NULL):
           data(item),left(L),right(R){}
       ~Node(){}
    };

    //用于中序和后序遍历的非递归实现算法中
    //TimesPop用来记录根结点的出栈次数，中序遍历中应该在第二次出栈时访问根结点
    //后序遍历应该在第三次出栈时访问根结点
    struct StNode{
        Node * node;
        int TimesPop;

        StNode(Node * N=NULL):node(N),TimesPop(0) { }
    };

    Node *root;   //二叉树的根结点

public:
    BinaryTree():root(NULL){}  //构造空二叉树
    BinaryTree(const Type & value){root=new Node(value);}

    ~BinaryTree(){clear();}

    Type getRoot()    const {return root->data;}
    Type getLeft()      const {return root->left->data;}
    Type getRight()    const {return root->right->data;}

    //makeTree(x,Tl,Tr)操作构建一棵以x为根结点，以Tl和Tr为左右子树的二叉树．
    //只需为x申请一个结点，让它的left指向Tl的根结点，right指向Tr的根结点．
    //由于在该函数中要修改被归并的左右子树的根，因此在参数设计时把它们设计成引用传递
    void makeTree(const Type &x,BinaryTree &lt,BinaryTree &rt){
        root=new Node(x,lt.root,rt.root);
        //如果不将这两棵树置为空树，就意味着有两棵二叉树对象共享了同一块空间，
        //lt和rt修改后当前构造的二叉树也就被修改了，所以在归并后将这两棵树的树根设为NULL，
        //表示以后不能再对这两棵树做操作了，只能对当前树的子树做操作
        lt.root=NULL;
        rt.root=NULL;
    }

    void delLeft(){
        BinaryTree temp=root->left;
        root->left=NULL;
        temp.clear();
    }

    void delRight(){
        BinaryTree temp=root->right;
        root->right=NULL;
        temp.clear();
    }

    bool isEmpty() const {return root==NULL;}

    //公有成员函数通过调用同名的私有成员函数实现它的功能，相当于一个包裹函数
    //因为如对前序遍历来说，对用户并不知道调用的函数需要参数，只知道自己需要遍历当前的二叉树对象
    //这样可以将实现过程封装，与用户隔开
    void clear(){
        if(root!=NULL)    clear(root);
        root=NULL;
    }

    int size() const {return size(root);}

    int height() const {return height(root);}

//递归实现前序遍历
//    void preOrder() const {
//        if(root!=NULL){
//            cout<<"\n前序遍历:";
//            preOrder(root);
//        }
//    }

//非递归实现前序遍历
    void preOrder() const {
        stack<Node *> s;
        Node *current;

        cout<<"前序遍历：";
        s.push(root);
        while(!s.empty()){
            current=s.top();
            s.pop();
            cout<<current->data;
            if(current->right!=NULL) s.push(current->right);    //后进先出
            if(current->left!=NULL) s.push(current->left);
        }
    }

    //非递归实现后序遍历
    void postOrder() const {
        stack<StNode> s;
        StNode  current(root);

        cout<<"后序遍历：";
        s.push(current);    //根结点入栈
        while(!s.empty()){
            current=s.top();
            s.pop();
            if(++current.TimesPop==3){ //第三次出栈，可以访问
                cout<<current.node->data;
                continue;    //左右子树已经访问完毕
            }
            s.push(current);
            if(current.TimesPop==1){ //第一次出栈
                if(current.node->left!=NULL)  s.push(StNode(current.node->left));
            }
            else{  //第二次出栈，左子树已经访问完毕
                if(current.node->right!=NULL)   s.push(StNode(current.node->right));
            }
        }
    }

    //非递归实现中序遍历
    void midOrder() const {
        stack<StNode> s;
        StNode  current(root);

        cout<<"中序遍历：";
        s.push(current);  //根结点入栈
        while(!s.empty()){
            current=s.top();
            s.pop();
            if(++current.TimesPop==2){    //第二次出栈，可以被访问
                cout<<current.node->data;
                if(current.node->right!=NULL)  s.push(StNode(current.node->right));
            }
            else{    //第一次出栈，不能被访问，重新入栈
                s.push(current);
                if(current.node->left!=NULL)  s.push(StNode(current.node->left));
            }
        }
    }


//    void postOrder() const {
//        if(root!=NULL){
//            cout<<"\n后序遍历:";
//            postOrder(root);
//        }
//    }

//    void midOrder() const {
//        if(root!=NULL){
//            cout<<"\n中序遍历:";
//            midOrder(root);
//        }
//    }

    void createTree(Type flag){
        queue<Node *> que;
       // linkQueue<Node *> que;  自己实现的队列链表结构
        Node *tmp;
        Type x,ldata,rdata;

        //创建树，输入flag表示空
        cout<<"\n输入根结点";
        cin>>x;
        root=new Node(x);
        que.push(root);

        while(!que.empty()){
            tmp=que.front();
            que.pop();
            cout<<"\n输入"<<tmp->data<<"的两个儿子("<<flag<<"表示空结点):";
            cin>>ldata>>rdata;
            if(ldata!=flag) que.push(tmp->left=new Node(ldata));
            if(rdata!=flag) que.push(tmp->right=new Node(rdata));
        }
        cout<<"create completed! \n";
    }

private:
    void clear(Node *t){
        if(t->left!=NULL) clear(t->left);
        if(t->right!=NULL) clear(t->right);
        delete t;
    }

    int size(Node *t) const {
        if(t==NULL) return 0;
        return 1+size(t->left)+size(t->right);
    }

    int height(Node *t) const {
        if(t==NULL) return 0;
        return max(1+height(t->left),1+height(t->right));
    }

    void preOrder(Node *t) const{
        if(t!=NULL){
            cout<<t->data<<'\t';
            preOrder(t->left);
            preOrder(t->right);
        }
    }

    void postOrder(Node *t) const {
        if(t!=NULL){
            postOrder(t->left);
            postOrder(t->right);
            cout<<t->data<<'\t';
        }
    }

    void midOrder(Node *t) const {
        if(t!=NULL){
            midOrder(t->left);
            cout<<t->data<<'\t';
            midOrder(t->right);
        }
    }
};
int main(){
    BinaryTree<char> tree,tree1('M'),tree2;

    tree.createTree('@');

    cout<<"高度为："<<tree.height()<<endl;
    cout<<"规模为："<<tree.size()<<endl;
    tree.preOrder();
    tree.midOrder();
    tree.postOrder();

    tree2.makeTree('Y',tree,tree1);
    cout<<endl;
    cout<<"高度为："<<tree2.height()<<endl;
    cout<<"规模为："<<tree2.size()<<endl;
    tree2.preOrder();
    tree2.midOrder();
    tree2.postOrder();

    return 0;
}
