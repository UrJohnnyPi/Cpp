/*
简单工厂模式
*/

class Ball{
    public :
        Ball(){}
        virtual void display() = 0;
        virtual void play() = 0;
};

class Basketball : public Ball{
    public :
        Basketball() : Ball(){}
        void display(){
            cout << "Basketball" << endl;
        }
        void play(){
            cout << "拍打" << endl;
        }
};

class Football : public Ball{
    public :
        Football() : Ball(){}
        void display(){
            cout << "Football" << endl;
        }
        void play(){
            cout << "踢" << endl;
        }
};

class Factory{
    public :
        Ball * ball(string str){
            Ball * ptr = nullptr;
            switch(str){
                case "Basketball" : 
                    ptr = new Basketball();
                    break;
                case "Football" :
                    ptr = new Football();
                    break;
                default :
                    exit(1);
            }
            return ptr;
        }
}

int main(){
    Factory * fac = new Factory;
    string str;
    cin >> str;
    Ball * ball = fac(str);
    ball->display();
    ball->play();
    return 0;
}