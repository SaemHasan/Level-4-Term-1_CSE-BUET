#include "Utils_zBuffer.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    //input file setup
    FILE *stagefile = NULL;
    ifstream configfile;
    string defaultstageFile = "output/stage3.txt";
    string defaultconfigFile = "input/config.txt";

    //setup config file
    
    if(argc>2){
        configfile.open(argv[2]);
        cout<<"Using config file: "<<argv[2]<<endl;
    }
    else{
        cout<<"no input file provided"<<endl;
        cout<<"openning default files...."<<endl;
        configfile.open(defaultconfigFile.c_str());
    }

    if(!configfile.is_open()){
        cout<<"Error opening input file"<<endl;
        return 1;
    }

    //read config file
    configfile>>SCREEN_WIDTH>>SCREEN_HEIGHT;
    configfile>>LEFT_LIMIT>>BOTTOM_LIMIT;
    RIGHT_LIMIT = -LEFT_LIMIT;
    TOP_LIMIT = -BOTTOM_LIMIT;
    configfile>>FRONT_Z>>REAR_Z;
    configfile.close();

    //setup stage file
    if(argc>2){
        stagefile = freopen(argv[1],"r",stdin);
        cout<<"opening stage file: "<<argv[1]<<endl;
    }
    else{
        cout<<"no stage file provided"<<endl;
        cout<<"openning default stage file...."<<endl;
        stagefile = freopen(defaultstageFile.c_str(),"r",stdin);
    }

    if(stagefile == NULL){
        cout<<"Error opening stage file"<<endl;
        return 1;
    }

    //read stage file
    while(true){
        Triangle t;
        cin>>t;
        if(t.points[0].length()==0 && t.points[1].length()==0 && t.points[2].length()==0){
            break;
        }
        triangles.push_back(t);
        // cout<<t;
    }
    cout<<"stage file read successfully"<<endl;
    fclose(stagefile);

    printDetails();

    //create output file
    ofstream zBuffer("output/z-buffer.txt");
    if(!zBuffer.is_open()){
        cout<<"Error opening output file"<<endl;
        return 1;
    }
    zBuffer<<fixed<<setprecision(6);

    //z buffer and image setup
    initializeBuffers();
    
    //procedure
    zBufferAlgorithm();

    cout<<"memory clear kora lagbe. vector gula delete korte hobe using vector<tempObject>().swap(tempVector);"<<endl;
    zBuffer.close();
    return 0;
}