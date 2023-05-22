/*#include<iostream>
#include<fstream>
#include<set>
#include<algorithm>
*/
#include<bits/stdc++.h>
#define asciiSIZE 130
using namespace std;

struct node{
    int ascii;
    int freq;
    node *lch;
    node *rch;
    inline bool operator < (const node &a)const{
        return freq > a.freq;
    }
};

void compress(void);
void decompress(void);
void getFilename( string *filename1, string *filename2);//转换文件名
void findFrequency( string loc);//统计字频并输出
void buildHuffmanTree(void);//构造哈夫曼树
void encode(void);//字符编码
//void encodeFile(string filename1,string filename2);//文件编码压缩,计算大小

priority_queue<node> priABC;//按字频由小到大存储
int intABC[asciiSIZE];//字频记录
node *root;//Huffman树根节点
string hufcode[asciiSIZE];//Huffman编码
//int deciHufcode[asciiSIZE];
int originSize,finalSize;

int main(void){
    while(1){
        char opt;
        printf("要对文件进行什么操作?\n1.压缩\n2.解压\n3.不操作，退出程序\n");
        scanf("%c",&opt);
        if( opt == '1' )compress();
        else if( opt == '2' )decompress();
        else if( opt == '3' )break;
        else{
            printf("没有此操作。是否退出程序?\n1.yes\n2.no\n");
            scanf("%c",&opt);
            if( opt == '1' )break;
        } 
    }
    return 0;

}
////////////////////////////////////////////////////////////////////
void compress(void){
    //找地址
    printf("输入文件名:\n");getchar();
    string filename1,filename2;
    getFilename( &filename1, &filename2);
    //文件存在与否
    ifstream file1(filename1,ios::in);
    if(file1)file1.close();else{printf("文件不存在\n");return ;}

    //存在，开始压缩
    printf("压缩中...\n");
    findFrequency( filename1 );//统计字频并输出
    buildHuffmanTree();//构造Huffman树
    encode();//编码并输出
    //encodeFile(filename1,filename2);//将压缩的1保存在2,计算文件大小
    printf("压缩完成!\n");
}
/////////////////////////////////////////////////////////////////

void decompress(void){

}
/////////////////////////////////////////////////////////////////
void findFrequency( string loc){
    ifstream file(loc,ios::in);
    while(!priABC.empty())priABC.pop();
    char c;
    int cnt=0;
    for(int i=0;i<asciiSIZE;i++)intABC[ i ] = 0;
    while( (c = file.get() ) && c != EOF ){
        if( intABC[ c ] == 0 )cnt++;
        intABC[ c ]++;
    }

    printf("频率统计:\n字符 | 出现次数:\n");
    for(int i=0;i<asciiSIZE;i++){
        if(intABC[ i ]){
            priABC.push( { i , intABC[ i ],NULL,NULL});
            switch ( i )
            {
            case 9:printf("\"\\t\"");break;
            case 10:printf("\"\\n\"");break;
            case 11:printf("\"\\v\"");break;
            case 12:printf("\"\\f\"");break;
            case 13:printf("\"\\r\"");break;
            //case 34:printf("\\"");break;
            //case 39:printf("\\'");break;
            case 92:printf("\"\\\" ");break;
            default:printf("\"%c\" ",char(i));break;
            }
            printf(" | %d\n",intABC[ i ]);
        }
    }
    file.close();
    
}

void getFilename( string *filename1, string *filename2){
    char ch = '0';
    (*filename1).clear(),(*filename2).clear();
    while(1){
        scanf("%c",&ch);
        if( ch == '\n' )break;
        else if( ch == '\\'){
            *filename1 += '/',*filename1 += '/';
            *filename2 += '/',*filename2 += '/';
        }else if( ch == '.' ){
            *filename1 += ".txt";
            *filename2 += ".huf";
            break;
        }else{
            *filename1 += ch;
            *filename2 += ch;
        }
    }
}

void buildHuffmanTree(){
    while(!priABC.empty()){
        //取出两最小权
        node ta,tb;
        ta = priABC.top();
        priABC.pop();
        tb = priABC.top();
        priABC.pop();
        //构造parent
        node tt;
        tt.ascii = 666;
        tt.freq = ta.freq + tb.freq ;
        tt.lch = new node;
        tt.lch->ascii = ta.ascii;
        tt.lch->freq = ta.freq;
        tt.lch->lch = ta.lch;
        tt.lch->rch = ta.rch;
        tt.rch = new node;
        tt.rch->ascii = tb.ascii;
        tt.rch->freq = tb.freq;
        tt.rch->lch = tb.lch;
        tt.rch->rch = tb.rch;
        if(priABC.empty())break;
        priABC.push( tt );
        root = &tt;
    }
}

void dfs(node *t,string tmp){
    if(t->lch == NULL){
        hufcode[ t->ascii ] = tmp;
        int nm = stoi(tmp);

    }else{
        tmp += "0";
        dfs( t->lch, tmp);
        tmp.erase(tmp.end()-1);
        tmp += "1";
        dfs( t->rch , tmp);
    }
}

void encode(void){
    //string tmp;
    //printf("字符及对应Huffman编码:\n字符 | 编码:\n");
    //node *t = root;
    dfs(root,"");
    
    for(int i=0;i<asciiSIZE;i++){
        if( !hufcode[ i ].empty()){
            //priABC.push( { i , intABC[ i ],NULL,NULL});
            switch ( i )
            {
            case 9:printf("\"\\t\"");break;
            case 10:printf("\"\\n\"");break;
            case 11:printf("\"\\v\"");break;
            case 12:printf("\"\\f\"");break;
            case 13:printf("\"\\r\"");break;
            //case 34:printf("\\"");break;
            //case 39:printf("\\'");break;
            case 92:printf("\"\\\" ");break;
            default:printf("\"%c\" ",char(i));break;
            }
            //printf(" | %s\n",hufcode[i]);
            printf(" | ");
            cout<<hufcode[i]<<'\n';
        }
    }
    
}

/*
void encodeFile(string filename1,string filename2){
    ifstream file1(filename1,ios::in);
    ofstream file2(filename2,ios::out|ios::binary);
    finalSize = 0, originSize = 0;
    char c;
    string tmpstr;

    while (( c=file1.get() )&& c !=EOF )
    {
        originSize++;
        tmpstr += hufcode[ c ];
        while( tmpstr.size() >= 8 ){//8 bits = 1byte
            int n = 0;
            for(int i=7;i>=0;i--){
                n = n * 2 + (tmpstr[ i ] - '0');
            }
            for(int i=0;i<8;i++){
                tmpstr.erase(tmpstr.begin());
            }
            char tmpc = char(n);
            file2.write( &tmpc, sizeof(char));
        }
    }

    while(tmpstr.size()%8)tmpstr += "0";//后面补0
    while( !tmpstr.empty() ){
            int n = 0;
            for(int i=7;i>=0;i--){
                n = n * 2 + (tmpstr[ i ] - '0');
            }
            for(int i=0;i<8;i++){
                tmpstr.erase(tmpstr.begin());
            }
            char tmpc = char(n);
            file2.write( &tmpc, sizeof(char));
    }
    //写完了
    file1.close();
    file2.close();
    //获取二进制文件大小
    long l , m ;
    ifstream file(filename2, ios::in | ios::binary);
    l = file.tellg();
    file.seekg(0, ios::end);
    m = file.tellg();
    finalSize = m - l ;
    file.close();

    printf("压缩比:%.2f\n",( originSize )/float( finalSize ));
}
*/