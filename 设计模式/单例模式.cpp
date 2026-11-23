/*
单例模式
*/

class Single{
    public :
        static Single * SingleCreator(){
            if(ptr == nullptr) ptr = new Single;
            return ptr;
        }
    private :
        Single(){}
        static Single * ptr;
};
Single * Single::ptr = nullptr;

int main(){
    Single * sing = Single::SingleCreator();
    return 0;
}

//多线程下的单例模式
class Single{
    public :
        static Single * SingleCreator(){
            if(ptr == nullptr){
                mut.lock();
                if(ptr == nullptr) ptr = new Single;
                mut.unlock();
            }
            return ptr;
        }
    private :
        Single(){}
        static Single * ptr;
        mutex mut;
};
Single * Single::ptr = nullptr;