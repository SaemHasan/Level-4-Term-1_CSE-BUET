#include "UtilsMatrix.h"

void testPOint(){
    cout<<"hello world"<<endl;
    Point p = Point(1,2,3);
    Point p1 = Point(4,5,6);
    Point p2 = p + p1;
    cout<<p2;
    Point P3 = p1 - p;
    cout<<P3;
    Point P4 = p * 2;
    cout<<P4;
    Point P5 = p / 2;
    cout<<P5;
    Point p6 = p;
    cout<<p6;
    Point P7 = -p1;
    cout<<P7;
    
    cout<<p1.dot(p)<<endl;

    cout<<p1.cross(p);

}

void testMatrix(){
    cout<<Matrix::identityMatrix();
    cout<<Matrix::translationMatrix(1,2,3);
    cout<<Matrix::translationMatrix(Point(5,6,7));
    cout<<Matrix::scalingMatrix(Point(1,2,3));
}

Point transformPoint(Matrix m, Point p){
    Matrix pointMatrix = Matrix::pointMatrix(p);
    Matrix result = m * pointMatrix;
    // cout<<fixed<<setprecision(5)<<result<<endl;
    return result.getPoint();
}


Matrix viewTransformationMatrix(Point eye, Point look, Point up){
    Point l = look - eye;
    l.normalize();
    Point r = l.cross(up);
    r.normalize();
    Point u = r.cross(l);

    Matrix T = Matrix::translationMatrix(-eye);
    Matrix R = Matrix::rotationMatrix(r, u, -l);

    Matrix V = R * T;

    return V;
}

int main(int argc,char* argv[]){
    
    //file setup
    FILE *infile = NULL;
    string defaultfile = "input/scene.txt";

    if(argc>1){
        infile = freopen(argv[1],"r",stdin);
    } 
    else if(argc==1){
        cout<<"no input file provided"<<endl;
        cout<<"openning default file: "<<defaultfile<<endl;
        infile = freopen(defaultfile.c_str(),"r",stdin);
    }

    if (infile == NULL) {
        cout<<"Error opening file"<<endl;
        return 1;
    }

    //create output file
    ofstream stage1("output/stage1.txt");
    ofstream stage2("output/stage2.txt");
    ofstream stage3("output/stage3.txt");


    if(!stage1.is_open() || !stage2.is_open() || !stage3.is_open()){
        cout<<"Error opening output file"<<endl;
        return 1;
    }


    int numberOfDecimalPlaces = 7;
    stage1<<fixed<<setprecision(numberOfDecimalPlaces);
    stage2<<fixed<<setprecision(numberOfDecimalPlaces);
    stage3<<fixed<<setprecision(numberOfDecimalPlaces);

    //Read from file
    Point eye, look, up;
    double fovY, aspectRatio, near, far;

    cin>>eye>>look>>up;
    // cout<<eye<<look<<up;
    cin>>fovY>>aspectRatio>>near>>far;
    // cout<<fovY<<" "<<aspectRatio<<" "<<near<<" "<<far<<endl;

    //create view transformation matrix
    Matrix ViewMatrix = viewTransformationMatrix(eye, look, up);

    //create projection transformation matrix
    Matrix ProjectMatrix = Matrix::projectionMatrix(fovY, aspectRatio, near, far); //fovY, aspectRatio, near, far

    //initialize empty stack S
    stack<Matrix> S;
    S.push(Matrix::identityMatrix());

    //for push and pop
    stack< stack<Matrix> > stackofS;

    while(true){
        string command;
        cin>>command;

        if(command == "triangle"){
            Point p1, p2, p3;
            cin>>p1>>p2>>p3;
            
            Point p[]={p1,p2,p3};
            // stage1<<"triangle "<<p1<<p2<<p3<<endl;
            for(int i = 0; i < 3; i++){
                Point s1 = transformPoint(S.top(), p[i]);
                stage1<<s1;
                Point s2 = transformPoint(ViewMatrix, s1);
                stage2<<s2;
                Point s3 = transformPoint(ProjectMatrix, s2);
                stage3<<s3;
            }
            stage1<<endl;
            stage2<<endl;
            stage3<<endl;
        }

        else if(command == "translate"){
            Point t;
            cin>>t;
            Matrix T = Matrix::translationMatrix(t);
            // cout<<t;
            // cout<<T;
            S.push(S.top() * T);
        }

        else if(command == "scale"){
            Point s;
            cin>>s;
            Matrix Sc = Matrix::scalingMatrix(s);
            S.push(S.top() * Sc);
        }

        else if(command == "rotate"){
            double angle;
            cin>>angle;
            Point r;
            cin>>r;
            Matrix R = Matrix::rotationMatrix(angle, r);
            S.push(S.top() * R);
        }

        else if(command == "push"){
            stackofS.push(S);
        }

        else if(command == "pop"){
            S = stackofS.top();
            stackofS.pop();
        }
        else if(command == "end"){
            break;
        }
        else{
            cout<<"Invalid command"<<endl;
        }
    }

    // cout<<"done\n";

    stage1.close();
    stage2.close();
    stage3.close();
    return 0;

}