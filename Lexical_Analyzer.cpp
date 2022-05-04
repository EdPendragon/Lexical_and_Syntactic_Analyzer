#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;
#define MAXN 500
vector<string> vec; //用来存放token表的行数、关键字、内容
int nvec=0;
struct F  //从K到I的映像，如A-(a)>B
{
  char zuo; //式子左边，即A
  char zho; //式子中间，即a
  char you; //式子右边，即B
};
struct NFA  //NFA数据结构
{
  char K[MAXN]={0}; //结点集合
  int nk; //结点个数
  F f[MAXN]={0};  //K到I的映射，即NFA图上各结点之间的“边”
  int nf; //“边”的个数
} nfa;
void initNFA(){  //初始化NFA
  nfa.K[0]='Z';
  nfa.nk=1; //nfa的K[0]设为结束结点
  nfa.nf=0; //初始有0条边
}
void create_NFA(string str){  //str为当前输入的文法,A->b则f(A,b)=Z，A->aB则f(A,a)=B
  char Left=str[0]; //Left即‘左’，代表箭头左边的结点
  for(int i=0;i<nfa.nk;i++){  //看Left是否已在K中，若不在则放入
    if(nfa.K[i]==Left) break; //假如K中某结点等于Left则不用放，退出循环即可
    if(i==(nfa.nk-1)){  //假如i等于nk-1说明K中所有结点都不等于Left
      nfa.K[nfa.nk]=Left; //在K数组末尾加入结点Left
      nfa.nk++;
    }
  }
  int tk=3; //0是Left，1和2是-和>，3代表右边的第一个字符
  while(str[tk]){ //遍历箭头后字符串的每一个字符，取其中的大写字母
    if(str[tk]<'A'||str[tk]>'Z'){ //不是大写字母，不是结点
      tk++; //换下一个字符
      continue;
    }
    for(int i=0;i<nfa.nk;i++){  //看str[tk]是否已在K中，若不在则放入       其实和判断Left一样
      if(nfa.K[i]==str[tk]) break; //假如K中某结点等于str[tk]则不用放，退出循环即可
      if(i==(nfa.nk-1)){  //假如i等于nk-1说明K中所有结点都不等于str[tk]
        nfa.K[nfa.nk]=str[tk]; //在K数组末尾加入结点str[tk]
        nfa.nk++;
      }
    }
    tk++; //切换到下一个字符
  }
  tk=3; //回到箭头后的第一个字符
  string nows;  //now string。。。。。。。当前录入的字符串的意思
  while(str[tk]){ //全部遍历，把被|分割开的每一段取出
    if(str[tk]!='|'){ //假如不是'|'则放入nows里
      nows+=str[tk];
      tk++; //切换到下一个字符
      continue;
    }
    //能走到下一行本身就说明str[tk]是'|'，否则已经在上一行continue
    if(nows.length()==1){ //只有一个字符，要看是大写还是小写
      if(nows[0]>='A'&&nows[0]<='Z'){  //假如是大写字母，即无条件转移结点
        nfa.f[nfa.nf].zuo=Left; //边的左边是Left，即箭头左侧的结点
        nfa.f[nfa.nf].zho='@';  //边的中间是'@'，即无条件转移
        nfa.f[nfa.nf].you=nows[0];  //边的右边是nows[0],由于nows长度为1，因此是0
        nfa.nf++; //边数加一
      }
      else{ //不是大写字母，说明是小写字母或其它字符，直接连到结束结点
        nfa.f[nfa.nf].zuo=Left; //边的左边是Left，即箭头左侧的结点
        nfa.f[nfa.nf].zho=nows[0];  //边的中间是nows[0]，由于nows长度为1，因此是0
        nfa.f[nfa.nf].you=nfa.K[0]; //K[0]是Z即结束结点
        nfa.nf++; //边数加一
      }
    }
    else{ //否则长度为2，第一个为终结符，第二个为终结符
      nfa.f[nfa.nf].zuo=Left; //边的左边是Left，即箭头左侧的结点
      nfa.f[nfa.nf].zho=nows[0];  //边的中间是nows[0]即字符串中的终结符
      nfa.f[nfa.nf].you=nows[1];  //边的右边是nows[1]即字符串中的非终结符
      nfa.nf++; //边数加一
    }
    tk++; //跳过'|'，直接到下一个字符
    nows="";  //nows清空
  } //由于正规文法每一行结尾不是'|'，结束循环时nows并不为空，需要再处理一次
  //其实while改成do while就可以了，但懒得改，复制了
  if(nows.length()==1){ //只有一个字符，要看是大写还是小写
    if(nows[0]>='A'&&nows[0]<='Z'){  //假如是大写字母，即无条件转移结点
      nfa.f[nfa.nf].zuo=Left; //边的左边是Left，即箭头左侧的结点
      nfa.f[nfa.nf].zho='@';  //边的中间是'@'，即无条件转移
      nfa.f[nfa.nf].you=nows[0];  //边的右边是nows[0],由于nows长度为1，因此是0
      nfa.nf++; //边数加一
     }
    else{ //不是大写字母，说明是小写字母或其它字符，直接连到结束结点
      nfa.f[nfa.nf].zuo=Left; //边的左边是Left，即箭头左侧的结点
      nfa.f[nfa.nf].zho=nows[0];  //边的中间是nows[0]，由于nows长度为1，因此是0
      nfa.f[nfa.nf].you=nfa.K[0]; //K[0]是Z即结束结点
      nfa.nf++; //边数加一
    }
  }
  else{ //否则长度为2，第一个为终结符，第二个为终结符
    nfa.f[nfa.nf].zuo=Left; //边的左边是Left，即箭头左侧的结点
    nfa.f[nfa.nf].zho=nows[0];  //边的中间是nows[0]即字符串中的终结符
    nfa.f[nfa.nf].you=nows[1];  //边的右边是nows[1]即字符串中的非终结符
    nfa.nf++; //边数加一
  }
}
void showNFA(){
  cout<<endl;
  cout<<"生成的NFA如下："<<endl;
  cout<<"起点        终结符        终点"<<endl; //表格的形式展示结点之间关系
  int nnfa=0;
  for(int i=0;i<nfa.nf;i++){
    cout<<nfa.f[i].zuo<<"             "<<nfa.f[i].zho<<"             "<<nfa.f[i].you;
    if(i!=(nfa.nf-1)) cout<<endl;
    nnfa++;
  }
  cout<<endl<<"其中S为开始结点,Z为结束结点"<<",NFA共"<<nnfa<<"条边";
  cout<<endl;
}
void load_Grammar(){  //读取文法文件，并启动create_NFA函数，随后展示
  initNFA();  //NFA首先初始化，然后再往里读取文法
  ifstream myfile;  //用于打开文法文件
  myfile.open("grammar_3.txt"); //打开文件夹内的文法文件
  // myfile.open("d://vscode//vsCode//Curriculum_design//grammar_3.txt"); //打开文件夹内的文法文件
  if (!myfile.is_open()){ //假如打开文法失败，报错并退出函数
		cout << "无法打开文件" << endl;
		return;
	}
  string nows;  //当前字符串
  while(getline(myfile,nows)){
    if(nows[0]=='$') continue;  //美元符代表注释，跳过
    create_NFA(nows);
  }
  showNFA();
  myfile.close();
}
struct FF{  //DFA的转换函数的数据结构
  char zu[MAXN]={0};  //以下三个分别是式子的左、中、右,左、右是NFA结点的集合，中是终结符
  char zh;
  char yo[MAXN]={0};
};
struct K{
  char K[MAXN]={0};
};
struct DFA{ //DFA数据结构
  K Kk[MAXN]={0}; //结点集合的集合
  int nk=0;  //结点集合数量
  FF f[MAXN]={0}; //转换函数的集合，即“边”
  int nf=0; //“边”的个数
} dfa;
void CopyS(char *a,char *b){  //将b的内容复制到a中，工具函数
  for(int k=0;b[k];k++) a[k]=b[k];
}
void SortS(char *S){  //将字符数组排序，不多解释,工具函数
  for(int i=0;S[i+1];i++){
    for(int j=i+1;S[j];j++){
      if(S[i]<S[j]) continue;
      else{
        char tem=S[i];
        S[i]=S[j];
        S[j]=tem;
      }
    }
  }
}
bool equalS(char *S1,char *S2){ //判断两个字符数组是否等价，不多解释，工具函数
  int n1=0,n2=0;
  for(n1=0;S1[n1];n1++) ;
  for(n2=0;S2[n2];n2++) ;
  if(n1!=n2) return false;
  for(int i=0;i<n1;i++){
    if(S1[i]!=S2[i]) return false;
  }
  return true;
}
bool equalSS(char *A,string B){ //用于判断一个字符数组与一字符串是否等价，工具函数
  int nb=B.length();
  int na=0;
  for(na=0;A[na];na++) ;
  if(na!=nb) return false;
  for(int i=0;i<na;i++){
    if(A[i]!=B[i]) return false;
  }
  return true;
}
void closure(char* Sl,char *Sr){  //Sl指的是函数得到的子集，Sr指的是输入函数的子集，求闭包
  int l=0;  //记录Sl中的元素个数
  for(int i=0;Sr[i];i++){ //先把Sr内的元素自己放进去
    Sl[l]=Sr[i];
    l++;
  }
  for(int i=0;Sl[i];i++){ //遍历Sl中每个元素，把能通过'@'转到的非终结符放入Sl中
    for(int j=0;j<nfa.nf;j++){  //遍历每一条边
      if((nfa.f[j].zuo==Sl[i])&&(nfa.f[j].zho=='@')){ //假如有一条边起点为Sl[i]，且边上是'@'
        char temr=nfa.f[j].you; //取边的右边结点
        bool pan=false;
        for(int k=0;Sl[k];k++){ //判断该结点是否已经在Sl中
          if(Sl[k]==temr){  //假如结点已经在Sl中，pan取true，否则默认是false
            pan=true;
            break;
          }
          else continue;
        }
        if(pan) continue; //假如已在Sl中，判断下一条边
        Sl[l]=temr; //否则将temr即右边的结点放到Sl中
        l++;
      }
      else continue;  //假如该边不满足条件则跳过,看下一条边
    }
  }
  SortS(Sl);  //所有该放的结点都放完之后拍一下序，以便对比数组是否相等
}
void MoveS(char *A,char a,char *B){ //move(A,a)，为了防止撞关键字大写了一下，加了个S,B储存结果
  int nb=0; //记录B中元素个数
  for(int i=0;A[i];i++){  //遍历A中的每个结点
    for(int j=0;j<nfa.nf;j++){  //对每个结点，遍历nfa中每条边
      if((nfa.f[j].zuo==A[i])&&(nfa.f[j].zho==a)){  //假如边左侧是A[i]且终结符为a
        B[nb]=nfa.f[j].you; //把边右侧结点存入B中
        nb++; //nb加一
      }
      else continue;
    }
  }
  SortS(B); //把需要存的全存完之后把B排列一遍
}
void getTerminator(char *S){  //取得全部终结符，用于子集法构造DFA，工具函数
  int s=0;
  for(int i=0;i<nfa.nf;i++){  //遍历每一条边
    bool pan=false;
    for(int j=0;S[j];j++){
      if(nfa.f[i].zho==S[j]){
        pan=true;
        break;
      }
      else continue;
    }
    if(pan) continue;
    else{
      S[s]=nfa.f[i].zho;
      s++;
    }
  }
}
void create_DFA(){  //根据已生成的NFA生成DFA，使用子集法
  char Zero[MAXN]={'S'};  //代表头结点
  closure(dfa.Kk[0].K,Zero); //求头节点的闭包，成为第一个子集
  dfa.nk=1; //刚开始只有一个dfa结点，就是dfa.K[0]
  char Ter[MAXN]={0}; //用来存放所有终结符
  getTerminator(Ter); //取得所有终结符
  for(int i=0;i<dfa.nk;i++){  //用一个整型变量i来代替“标记”，i走到哪个位置说明哪个子集被标记处理了
    for(int j=0;Ter[j];j++){  //每个终结符处理一遍
      char TemS[MAXN]={0};  //工具子集
      MoveS(dfa.Kk[i].K,Ter[j],TemS);  //求move(dfa.K[i],Ter[j])
      char PointS[MAXN]={0};  //目标子集
      closure(PointS,TemS); //求-closure(move(dfa.K[i],Ter[j]))，储存在PointS中
      if(!(PointS[0])) continue; //假如PointS第一个字符为空，直接看下一个终结符
      bool pan=false;
      for(int t=0;t<dfa.nk;t++){  //遍历一遍dfa.K，看PointS是否已经在dfa.K中
        if(equalS(dfa.Kk[t].K,PointS)){  //假如存在于PointS相同的，pan为true，推出遍历
          pan=true;
          break;
        }
        else continue;  //否则继续遍历
      }
      if(pan){ //假如pan==true，说明PointS已在K中，只需加一条边即可
        if(Ter[j]=='@') continue; //假如边上是'@'，则不存
        CopyS(dfa.f[dfa.nf].zu,dfa.Kk[i].K); //f[dfa.nf]的左侧就是dfa.K[i]代表的子集
        dfa.f[dfa.nf].zh=Ter[j];  //f[dfa.nf]的中间就是Ter[j]代表的终结符
        CopyS(dfa.f[dfa.nf].yo,PointS); //f[dfa.nf]的右侧就是PointS代表的子集
        dfa.nf++; //dfa边数加一
      }
      else{ //否则说明得到的子集不在K中，需要把结点放进去，并加入一条边
        if(Ter[j]=='@') continue; //假如边上是'@'，则不存
        CopyS(dfa.f[dfa.nf].zu,dfa.Kk[i].K); //f[dfa.nf]的左侧就是dfa.K[i]代表的子集
        dfa.f[dfa.nf].zh=Ter[j];  //f[dfa.nf]的中间就是Ter[j]代表的终结符
        CopyS(dfa.f[dfa.nf].yo,PointS); //f[dfa.nf]的右侧就是PointS代表的子集
        dfa.nf++; //dfa边数加一
        CopyS(dfa.Kk[dfa.nk].K,PointS);  //加入完边之后把PointS结点放入K中
        dfa.nk++; //结点（子集）数加一
      }
    }
  }
}
void showDFA(){ //展示生成的DFA
  cout<<endl<<"生成的DFA如下："<<endl;
  cout<<"起点"<<"        "<<"终结符"<<"        "<<"终点"<<endl;
  int ndfa=0;
  for(int i=0;i<dfa.nf;i++){  //遍历每一条边
    ndfa++;
    for(int j=0;j<dfa.nk;j++){
      if(equalS(dfa.Kk[j].K,dfa.f[i].zu)) cout<<j; //用数字来代表数组，用数组在K的次序代表序号
      else continue;
    }
    cout<<"             "<<dfa.f[i].zh<<"            ";  //输出终结符
    for(int j=0;j<dfa.nk;j++){
      if(equalS(dfa.Kk[j].K,dfa.f[i].yo)) cout<<j; //用数字来代表数组，用数组在K的次序代表序号
      else continue;
    }
    if(i!=(dfa.nf-1)) cout<<endl;
  }
  cout<<endl<<"0为开始结点        ";
  for(int i=0;i<dfa.nk;i++){
    for(int j=0;dfa.Kk[i].K[j];j++){
      if(dfa.Kk[i].K[j]=='Z'){
        cout<<i<<' ';
        break;
      }
    }
  }
  cout<<"为结束结点"<<"，DFA共有"<<ndfa<<"条边";
}
bool is_EndNode(char *A){ //判断一子集是否是结束结点，不解释，工具函数
  for(int i=0;A[i];i++){
    if(A[i]=='Z') return true;
  }
  return false;
}
string del_FirstS(string A){  //得到string删去第一个字符的字符串，工具函数
  if(A.length()==0) return "";  //空串仍然返回空串
  else{
    string tem="";
    for(int i=1;i<A.length();i++) tem+=A[i];
    return tem;
  }
}
bool distinguish_Word(int node,string word){  //识别某一单词word,出发起点为node
  if(word.length()==0)  return false; //假如word是空串，直接return false
  else if(word.length()==1){ //假如word长度为1
    for(int i=0;i<dfa.nf;i++){  //遍历dfa的每一条边，找出其中从node号结点起，边上是word[0]的边
      if(equalS(dfa.Kk[node].K,dfa.f[i].zu)&&dfa.f[i].zh==word[0]){ //找到边
        if(is_EndNode(dfa.f[i].yo)) return true;  //假如该边右边的结点是结束节点，返回true
        else continue;  //否则继续考虑其它边
      }
    } //for循环结束还没有return说明没有找到符合条件的边
    return false; //找不到符合条件的边，return false
  }
  else{ //word长度大于1
    char noww=word[0];  //当前要判断的字符
    string Nword="";
    Nword=del_FirstS(word); //取Nword为word删去第一个字符
    for(int i=0;i<dfa.nf;i++){  //递归调用每一条从node号结点出发，边上是noww的边
      if(equalS(dfa.Kk[node].K,dfa.f[i].zu)&&dfa.f[i].zh==noww){  //找到边
        int nex=0;  //代表该边右边结点号
        for(int j=0;j<dfa.nk;j++){
          if(equalS(dfa.Kk[j].K,dfa.f[i].yo)){  //找到子集内容与当前边右侧相同的结点
            nex=j;  //得到结点号
            break;
          }
          else continue;
        }
        if(distinguish_Word(nex,Nword)) return true;  //假如下一结点，Nword字符串识别成功
        else continue;  //否则看下一条边
      }
      else continue;  //边不符合条件，则看下一条边
    } //for循环结束说明没有该字符串不行
    return false;
  }
}
string create_SonS(string A,int a,int b){  //生成一个字符串A中a位置到b位置的子串，工具函数
  if(a<0) return "";
  if(b>=A.length()) return "";
  string tem="";
  for(int i=a;i<=b;i++) tem+=A[i];
  return tem;
}
string keyword[MAXN]={"main","cin","cout","auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","unsigned","union","void","volatile","while"};
bool isKeyword(string A){ //判断是否属于关键字，否则就是标识符，工具函数
  for(int i=0;i<35;i++){
    if(A==keyword[i]) return true;
    else continue;
  }
  return false;
}
char jie[MAXN]={',',';','(',')','[',']','{','}'};
bool isLimiter(char a){ //判断是否属于界符，工具函数
  for(int i=0;i<8;i++){
    if(jie[i]==a) return true;
    else continue;
  }
  return false;
}
char suan[MAXN]={'+','-','*','/','%','&','^','=','>','<'};
bool isOperator(char a){  //判断是否属于运算符，工具函数
  for(int i=0;i<10;i++){
    if(suan[i]==a) return true;
    else continue;
  }
  return false;
}
string printToken(string A,string B){  //输出Token表
  if(A[0]=='l'||A[0]=='e'||A[0]=='i'||A[0]=='_'){ //标识符与关键字
    if(isKeyword(B)){ //关键字
      cout<<"keyword"<<"        ";
      return "keyword        ";
    }
    else{ //不在关键字数组内，即标识符
      cout<<"identifier"<<"      ";
      return "identifier      ";
    }
  }
  else if(isLimiter(A[0])){ //界符
    cout<<"limiter"<<"         ";
    return "limiter         ";
  }
  else if(isOperator(A[0])){  //运算符
    cout<<"operator"<<"        ";
    return "operator        ";
  }
  else{ //剩下的无论普通数字还是科学计数法还是复数都属于数字
    cout<<"number"<<"          ";
    return "number          ";
  }
}
string charConnectS(char a,string b){ //在string b的头部加上a，生成新的字符串，工具函数
  string c="";
  c+=a;
  for(int i=0;i<b.length();i++) c+=b[i];
  return c;
}
void distinguish_Sentence(string SentencE,int line){ //识别某一行代码，即一个句子,line是行数
  string sentence=SentencE;
  for(int i=0;i<sentence.length();i++){ //首先对sentence进行处理
    if(sentence[i]!='e'&&sentence[i]!='i'){ //把除了i和e之外的字母变成l
      if((sentence[i]>='a'&&sentence[i]<='z')||(sentence[i]>='A'&&sentence[i]<='Z')) sentence[i]='l';
    }
    if(sentence[i]=='0') sentence[i]='o'; //0变成小o
    if(sentence[i]>='1'&&sentence[i]<='9') sentence[i]='z'; //正整数（1-9）变成z
  }
  int head=0,now=0,pre=0; //三个int变量起到指针作用，head到pre代表前一个能识别的字符串，head到now代表当前识别的字符串
  while(now<sentence.length()){ //遍历整个输入的句子
    if(create_SonS(sentence,head,now)==" "){  //假如是空的，说明head与now在同一位置且当前位置为空的，跳过
      now++;
      head++;
      pre++;
      continue;
    }
    while(distinguish_Word(0,create_SonS(sentence,head,now))){  //只要head到now还能识别
      pre=now;  //让pre等于now
      now++;  //now指向下一个字符
      if(sentence[now]=='.') now++; //以下四种情况用于识别小数、科学计数法与复数
      if((sentence[now]=='+'||sentence[now]=='-')&&(sentence[now+1]=='i'||sentence[now+1]=='z')) now++;
      if(sentence[now]=='z'&&sentence[now+1]=='i') now++;
      if((sentence[now]=='e')&&(sentence[now+1]=='z')) now++;
    } //while结束说明head到now已无法匹配
    if((sentence[head]=='z'||sentence[head]=='o')&&(sentence[now]=='l'||sentence[now]=='_')){ //用来特殊识别数字开头的非法标识符
      char saveChar=SentencE[head];
      head++;
      while(distinguish_Word(0,create_SonS(sentence,head,now))){
        pre=now;
        now++;
      } //while结束说明head到now已无法匹配
      cout<<line<<"        "<<"wrong"<<"          "<<saveChar<<create_SonS(SentencE,head,pre)<<endl;
      vec.insert(vec.begin()+nvec,to_string(line)); //输出wrong并把结果存进vec内
      nvec++;
      vec.insert(vec.begin()+nvec,"wrong        ");
      nvec++;
      string toolS=charConnectS(saveChar,create_SonS(SentencE,head,pre));
      vec.insert(vec.begin()+nvec,toolS);
      nvec++;
      head=now;
      pre=now;
      continue;
    }
    cout<<line<<"        ";
    vec.insert(vec.begin()+nvec,to_string(line)); //把能识别的部分放进vec内
    nvec++;
    vec.insert(vec.begin()+nvec,printToken(create_SonS(sentence,head,pre),create_SonS(SentencE,head,pre)));
    nvec++;
    cout<<create_SonS(SentencE,head,pre)<<"\n";
    vec.insert(vec.begin()+nvec,create_SonS(SentencE,head,pre));
    nvec++;
    head=now; //每放进去一个就重新定位一次，让head和pre等于now
    pre=now;
  }
}
void distinguish_File(){
  ifstream myfile;  //用于打开源代码文件
  myfile.open("test.txt");
  string tem;
  int line=1;
  while(getline(myfile,tem)){ //取得源代码的每一行
    distinguish_Sentence(tem,line);
    tem="";
    line++; //别忘了行数++
  }
  myfile.close(); //关闭文件以保存结果
}
int main(){
  load_Grammar(); //加载文法
  create_DFA();   //创建dfa
  showDFA();  //展示dfa，可注释掉
  cout<<endl;
  distinguish_File(); //识别代码文件
  ofstream fout;  //用于向token文件输出
  fout.open("token.txt");  //打开文件
  int tem=0;
  for(int i=0;i<vec.size();i++){
    fout<<vec[i]; //为了防止内容丢失，先全部存在vec中，一起输出
    tem++;
    if(tem!=3) fout<<"        ";
    if(tem==3) fout<<"\n";  //每三个内容就换一次行
    if(tem==3) tem=0;
  }
  fout.close(); //关闭以保存文件
  getchar();
  return 0;
}