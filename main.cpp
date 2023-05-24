/*#include<iostream>
#include<fstream>
#include<set>
#include<algorithm>
*/
#include<bits/stdc++.h>
#define asciiSIZE 260
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
struct dnode
{
    char c;
    string str;
};
typedef dnode dnode;

void compress(void);//压缩
void decompress(void);//解压
bool getFilename( int op, string *filename1, string *filename2);//转换文件名
void findFrequency( string loc);//统计字频并输出
void buildHuffmanTree(void);//构造哈夫曼树
void encode( node rt);//对字符进行Huffman编码
void encodeFile(string filename1,string filename2);//文件压缩并计算大小
void decodeFile(string filename1,string filename2);//文件解压

int numberofchar = 0;//出现的不同字符数量
int intABC[asciiSIZE];//字频记录 findFrequency()
priority_queue<node> priABC;//按字频由小到大存储 findfrequency(),buildHuffmanTree()
node root;//Huffman树根节点
string hufcode[asciiSIZE];//Huffman编码
vector<dnode> deHufcode[80];//Huffman解码 不同字符数量不超过80 26*2+10+\r' ' ,.'"?!


int main(void){
    /*
    while(1){
        int opt = 0 ;
        printf("要对文件进行什么操作?\n1.压缩\n2.解压\n3.结束程序\n");
        scanf("%d",&opt);getchar();
        if( opt == 1 )compress();
        else if( opt == 2 )decompress();
        else if( opt == 3 )break;
        else{
            printf("没有此操作。是否结束程序?\n1.yes\n2.no\n");
            scanf("%d",&opt);
            if( opt != 2 )break;} 
    }
    */
   
   while(1){
        printf("要对文件进行什么操作?\n1.压缩\n2.解压\n3.结束程序\n");
        string opt;
        //scanf("%s",&opt);
        cin>>opt;
        if( opt.compare("1") == 0 )compress();
        else if( opt.compare("2") == 0 )decompress();
        else if( opt.compare("3") == 0 )break;
        else{
            string abs;
            while(1){
                printf("没有此操作。是否结束程序?\n1.yes\n2.no\n");
                
                //scanf("%s",&abs);
                cin>>abs;
                if( abs.compare("1") == 0 || abs.compare("2") == 0)break;
            } 
            if( abs.compare("1") == 0 )break;
        }
    }
   
    
    printf("结束\n");
    return 0;

}
////////////////////////////////////////////////////////////////////
void compress(void){
    printf("输入待压缩文件名(地址):\n");getchar();
    string filename1,filename2;
    if(!getFilename( 1,&filename1, &filename2)){return ;};
    ifstream file1(filename1,ios::in);
    if(file1)file1.close();else{printf("文件不存在\n");return ;}

    printf("开始压缩...\n");
    findFrequency( filename1 );//统计字频并输出
    buildHuffmanTree();//构造Huffman树
    encode( root );//编码并输出
    encodeFile(filename1,filename2);//将压缩的1保存在2,计算文件大小
    printf("压缩完成!生成文件");
    cout<<filename2<<endl;
}
/////////////////////////////////////////////////////////////////
void decompress(void){
    printf("输入待解压文件名(地址):\n");getchar();
    string filename1,filename2;
    if(!getFilename( 2,&filename1, &filename2)){return ;};
    ifstream file(filename2,ios::in);
    if(file)file.close();else{printf("文件不存在\n");return ;}
    
    printf("开始解压...\n");
    decodeFile( filename1, filename2);
    printf("解压完成!生成文件");
    cout<<filename1<<endl;
}
/////////////////////////////////////////////////////////////////
void findFrequency( string loc){
    ifstream file(loc,ios::in);
    while(!priABC.empty())priABC.pop();
    priABC.push({7,1,NULL,NULL});//放进去'\7' ring bell,用作标识
    char c;
    numberofchar = 1;
    for(int i=0;i<asciiSIZE;i++)intABC[ i ] = 0;
    while( (c = file.get() ) && c != EOF ){
        intABC[ c ]++;
    }

    printf("频率统计:\n字符 | 出现次数:\n");
    for(int i=0;i<asciiSIZE;i++){
        if(intABC[ i ]){
            numberofchar++;
            priABC.push( { i , intABC[ i ],NULL,NULL});
            switch ( i )
            {
            case 7:printf("\"\\a\"");break;
            case 9:printf("\"\\t\"");break;
            case 10:printf("\"\\n\"");break;
            case 11:printf("\"\\v\"");break;
            case 12:printf("\"\\f\"");break;
            case 13:printf("\"\\r\"");break;
            //case 34:printf("\"\\\"\"");break;
            //case 39:printf("\"\\\'\"");break;
            //case 92:printf("\"\\\" ");break;
            default:printf("\"%c\" ",char(i));break;
            }
            printf(" | %d",intABC[ i ]);
            if( i == 7 )printf("这个用作标识");
            printf("\n");
            
        }
    }
    file.close();
    
}

bool getFilename( int op, string *filename1, string *filename2){
    char ch = '0';
    (*filename1).clear(),(*filename2).clear();
    do{
        scanf("%c",&ch);
    }while(!( ch >='a' && ch <='z' || ch>='A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == '_'));
    while(1){
        //scanf("%c",&ch);
        if( ch == '\n' )break;
        else if( ch == '\\'){
            *filename1 += '/',*filename1 += '/';
            *filename2 += '/',*filename2 += '/';
        }else if( ch == '.' ){
            if( op==1 )*filename1 += ".txt";
            else *filename1 += "(2).txt";

            *filename2 += ".huf";
            break;
        }else{
            *filename1 += ch;
            *filename2 += ch;
        }
        scanf("%c",&ch);
    }
    //清除.txt
    string excs;
    cin>>excs;
    getchar();
    if( (excs.compare("txt") == 0 && op == 1 ) || (excs.compare("huf") == 0) && op == 2)return true;
    if( op == 1)
        printf("文件错误,不是.txt文件\n");
    else printf("文件错误,不是.huf文件\n");
    return false;
}

void buildHuffmanTree(){
    //node tmp;
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
        root = tt;
        if(priABC.empty())break;
        priABC.push( tt );
    }
}

void dfs( node t ,string tmp){
    if(t.lch == NULL){
        hufcode[ t.ascii ] = tmp;
        //int nm = stoi(tmp);

    }else{
        tmp += "0";
        dfs( *(t.lch), tmp);
        tmp.erase(tmp.end()-1);
        tmp += "1";
        dfs( *(t.rch) , tmp);
    }
}

void encode( node rt ){
    printf("字符及对应Huffman编码:\n字符 | 编码:\n");
    dfs( rt , "" );
    
    for(int i=0;i<asciiSIZE;i++){
        if( !hufcode[ i ].empty()){
            //priABC.push( { i , intABC[ i ],NULL,NULL});
            switch ( i )
            {
            case 7:printf("\"\\a\"");break;
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
            cout<<hufcode[i];
            if( i == 7 )printf(" (这个作为文件正文结束标识)");
            printf("\n");
        }
    }
    
}

void encodeFile(string filename1,string filename2){
    int originSize = 0 ,finalSize = 0;
    ifstream file1(filename1,ios::in);
    ofstream file2(filename2,ios::out|ios::binary);
    char c;int n;

    //写开头编码信息 n (char lencode code)*n  比如:3 a 1 00000000 b 2 10000000 c 3 11000000
    c = char( numberofchar );
    file2.write( &c, sizeof( char ));originSize++;//几个编码字符
    for(int i=0;i<asciiSIZE;i++){
        if(!hufcode[ i ].empty()){
            c = char( i );
            file2.write( &c, sizeof( char ));originSize++;//字符
            c = hufcode[ i ].size();
            file2.write( &c, sizeof( char ));originSize++;//编码长度
            string str = hufcode[ i ];
            while(str.size() % 8)str += '0';//编码补0
            int byte = str.size() / 8;
            //n = 0;
            for(int ii=0;ii<byte;ii++){//编码
                n = 0 ; c = '\0';
                for(int k=0;k<8;k++){n <<= 1;n |= (str[ 0 ]- '0' );str.erase(str.begin());}//单个字节处理
                c = n ;
                file2.write( &c, sizeof( char ));originSize++;
            }
        }
    }
    
    //写文章主体
    string tmpstr;
    int cnt = 0;
    while (( c=file1.get() )&& c !=EOF )
    {
        cnt++;
        if(cnt > 300){
            cnt++;
        }
        originSize++;
        tmpstr += hufcode[ c ];
        while( tmpstr.size() >= 8 ){//8 bits = 1byte
            n = 0;
            for(int i = 0;i< 8 ;i++){
                n <<= 1;
                n |= (tmpstr[ 0 ]- '0' );
                tmpstr.erase(tmpstr.begin());
            }
            //if(n < 0)n +=256;
            c = char( n );
            file2.write( &c, sizeof(char));
        }
    }
    //写结尾
    tmpstr += hufcode[ 7 ];//标识
    while(tmpstr.size()%8)tmpstr += '0';//后面补0
    while( !tmpstr.empty() ){
            n = 0;
            for(int i = 0 ;i < 8 ; ++i){
                n <<= 1;
                n |= (tmpstr[ 0 ]- '0' );
                tmpstr.erase(tmpstr.begin());
            }
            c = char( n );
            file2.write( &c , sizeof(char));
    }
    //写完

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

    printf("压缩比(压缩文件大小/原文件大小):%.2f%\n",float(finalSize *100 )/( originSize ));
}

void decodeFile(string txtname,string hufname){
    char hufnamee[ hufname.length() ];
    hufname.copy( hufnamee , hufname.length() );
    FILE *readfile = fopen( hufnamee,"rb");
    ofstream writefile( txtname ,ios::out);//txt for write
    
    char c;
    char fl;
    int n;
    stack<char> tmp;
    string tmpstr;
    string str;//code 0101
    //c = file1.get();
    fscanf( readfile , "%c", &c );

    numberofchar = int(c);
    //开头处理
    for(int i=0;i<numberofchar;i++){
        //char ch = readfile.get();
        char ch,len;
        fscanf( readfile , "%c", &ch );
        //char len = readfile.get();
        fscanf( readfile , "%c", &len );
        int byte = ceil( ( len - '\0' ) / 8.0 );
        
        
        str.clear();
        for(int ii=0;ii<byte;ii++){//char to 01010
            //c = readfile.get();
            fscanf( readfile , "%c", &c );
            n = int( c );
            if( n < 0 ) n += 256;/////////////////////////////
            for(int i=0;i<8;i++){
                tmp.push( '0'+ (n&1) );
                n >>=1;
            }
            while(!tmp.empty()){
                str += tmp.top();
                tmp.pop();
            }
        }
        str.erase(str.begin()+int(len),str.end());
        deHufcode[ int(len) ].push_back({ ch , str });
    }
    str.clear();
    tmpstr.clear();
    //正文处理
    int cnt = 0;/////////
    //while((c = readfile.get())){
    while(1){
        fscanf( readfile , "%c", &c );
        n = int( c );
        if( n < 0 )n += 256;
        for(int i=0;i<8;i++){
            tmp.push( '0' + (n&1) );n >>=1;}//char to 0101
        while(!tmp.empty()){
            str += tmp.top();
            tmp.pop();
        }
        while(!str.empty()){
            tmpstr += str[0];
            str.erase(str.begin());
            int len = tmpstr.size();
            if(!deHufcode[ len ].empty()){
                int siz = deHufcode[ len ].size();
                for(int i=0;i<siz;i++){
                    if( tmpstr.compare( deHufcode[len][i].str) == 0){//找到
                        c = deHufcode[ len ][ i ].c;cnt++;
                        if( cnt > 310 ){/////
                            cnt ++;///////
                        }
                        if( c == '\7'){//结尾，停止
                            //readfile.close();
                            fclose( readfile );
                            writefile.close();
                            return ;
                        }
                        writefile.write( &c, sizeof(char));
                        tmpstr.clear();
                        break;
                    }
                }
            }
        }
    }
}