#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;
#define MAXN 500
vector<string> vec; //�������token����������ؼ��֡�����
int nvec=0;
struct F  //��K��I��ӳ����A-(a)>B
{
  char zuo; //ʽ����ߣ���A
  char zho; //ʽ���м䣬��a
  char you; //ʽ���ұߣ���B
};
struct NFA  //NFA���ݽṹ
{
  char K[MAXN]={0}; //��㼯��
  int nk; //������
  F f[MAXN]={0};  //K��I��ӳ�䣬��NFAͼ�ϸ����֮��ġ��ߡ�
  int nf; //���ߡ��ĸ���
} nfa;
void initNFA(){  //��ʼ��NFA
  nfa.K[0]='Z';
  nfa.nk=1; //nfa��K[0]��Ϊ�������
  nfa.nf=0; //��ʼ��0����
}
void create_NFA(string str){  //strΪ��ǰ������ķ�,A->b��f(A,b)=Z��A->aB��f(A,a)=B
  char Left=str[0]; //Left�����󡯣������ͷ��ߵĽ��
  for(int i=0;i<nfa.nk;i++){  //��Left�Ƿ�����K�У������������
    if(nfa.K[i]==Left) break; //����K��ĳ������Left���÷ţ��˳�ѭ������
    if(i==(nfa.nk-1)){  //����i����nk-1˵��K�����н�㶼������Left
      nfa.K[nfa.nk]=Left; //��K����ĩβ������Left
      nfa.nk++;
    }
  }
  int tk=3; //0��Left��1��2��-��>��3�����ұߵĵ�һ���ַ�
  while(str[tk]){ //������ͷ���ַ�����ÿһ���ַ���ȡ���еĴ�д��ĸ
    if(str[tk]<'A'||str[tk]>'Z'){ //���Ǵ�д��ĸ�����ǽ��
      tk++; //����һ���ַ�
      continue;
    }
    for(int i=0;i<nfa.nk;i++){  //��str[tk]�Ƿ�����K�У������������       ��ʵ���ж�Leftһ��
      if(nfa.K[i]==str[tk]) break; //����K��ĳ������str[tk]���÷ţ��˳�ѭ������
      if(i==(nfa.nk-1)){  //����i����nk-1˵��K�����н�㶼������str[tk]
        nfa.K[nfa.nk]=str[tk]; //��K����ĩβ������str[tk]
        nfa.nk++;
      }
    }
    tk++; //�л�����һ���ַ�
  }
  tk=3; //�ص���ͷ��ĵ�һ���ַ�
  string nows;  //now string����������������ǰ¼����ַ�������˼
  while(str[tk]){ //ȫ���������ѱ�|�ָ��ÿһ��ȡ��
    if(str[tk]!='|'){ //���粻��'|'�����nows��
      nows+=str[tk];
      tk++; //�л�����һ���ַ�
      continue;
    }
    //���ߵ���һ�б����˵��str[tk]��'|'�������Ѿ�����һ��continue
    if(nows.length()==1){ //ֻ��һ���ַ���Ҫ���Ǵ�д����Сд
      if(nows[0]>='A'&&nows[0]<='Z'){  //�����Ǵ�д��ĸ����������ת�ƽ��
        nfa.f[nfa.nf].zuo=Left; //�ߵ������Left������ͷ���Ľ��
        nfa.f[nfa.nf].zho='@';  //�ߵ��м���'@'����������ת��
        nfa.f[nfa.nf].you=nows[0];  //�ߵ��ұ���nows[0],����nows����Ϊ1�������0
        nfa.nf++; //������һ
      }
      else{ //���Ǵ�д��ĸ��˵����Сд��ĸ�������ַ���ֱ�������������
        nfa.f[nfa.nf].zuo=Left; //�ߵ������Left������ͷ���Ľ��
        nfa.f[nfa.nf].zho=nows[0];  //�ߵ��м���nows[0]������nows����Ϊ1�������0
        nfa.f[nfa.nf].you=nfa.K[0]; //K[0]��Z���������
        nfa.nf++; //������һ
      }
    }
    else{ //���򳤶�Ϊ2����һ��Ϊ�ս�����ڶ���Ϊ�ս��
      nfa.f[nfa.nf].zuo=Left; //�ߵ������Left������ͷ���Ľ��
      nfa.f[nfa.nf].zho=nows[0];  //�ߵ��м���nows[0]���ַ����е��ս��
      nfa.f[nfa.nf].you=nows[1];  //�ߵ��ұ���nows[1]���ַ����еķ��ս��
      nfa.nf++; //������һ
    }
    tk++; //����'|'��ֱ�ӵ���һ���ַ�
    nows="";  //nows���
  } //���������ķ�ÿһ�н�β����'|'������ѭ��ʱnows����Ϊ�գ���Ҫ�ٴ���һ��
  //��ʵwhile�ĳ�do while�Ϳ����ˣ������øģ�������
  if(nows.length()==1){ //ֻ��һ���ַ���Ҫ���Ǵ�д����Сд
    if(nows[0]>='A'&&nows[0]<='Z'){  //�����Ǵ�д��ĸ����������ת�ƽ��
      nfa.f[nfa.nf].zuo=Left; //�ߵ������Left������ͷ���Ľ��
      nfa.f[nfa.nf].zho='@';  //�ߵ��м���'@'����������ת��
      nfa.f[nfa.nf].you=nows[0];  //�ߵ��ұ���nows[0],����nows����Ϊ1�������0
      nfa.nf++; //������һ
     }
    else{ //���Ǵ�д��ĸ��˵����Сд��ĸ�������ַ���ֱ�������������
      nfa.f[nfa.nf].zuo=Left; //�ߵ������Left������ͷ���Ľ��
      nfa.f[nfa.nf].zho=nows[0];  //�ߵ��м���nows[0]������nows����Ϊ1�������0
      nfa.f[nfa.nf].you=nfa.K[0]; //K[0]��Z���������
      nfa.nf++; //������һ
    }
  }
  else{ //���򳤶�Ϊ2����һ��Ϊ�ս�����ڶ���Ϊ�ս��
    nfa.f[nfa.nf].zuo=Left; //�ߵ������Left������ͷ���Ľ��
    nfa.f[nfa.nf].zho=nows[0];  //�ߵ��м���nows[0]���ַ����е��ս��
    nfa.f[nfa.nf].you=nows[1];  //�ߵ��ұ���nows[1]���ַ����еķ��ս��
    nfa.nf++; //������һ
  }
}
void showNFA(){
  cout<<endl;
  cout<<"���ɵ�NFA���£�"<<endl;
  cout<<"���        �ս��        �յ�"<<endl; //������ʽչʾ���֮���ϵ
  int nnfa=0;
  for(int i=0;i<nfa.nf;i++){
    cout<<nfa.f[i].zuo<<"             "<<nfa.f[i].zho<<"             "<<nfa.f[i].you;
    if(i!=(nfa.nf-1)) cout<<endl;
    nnfa++;
  }
  cout<<endl<<"����SΪ��ʼ���,ZΪ�������"<<",NFA��"<<nnfa<<"����";
  cout<<endl;
}
void load_Grammar(){  //��ȡ�ķ��ļ���������create_NFA���������չʾ
  initNFA();  //NFA���ȳ�ʼ����Ȼ���������ȡ�ķ�
  ifstream myfile;  //���ڴ��ķ��ļ�
  myfile.open("grammar_3.txt"); //���ļ����ڵ��ķ��ļ�
  // myfile.open("d://vscode//vsCode//Curriculum_design//grammar_3.txt"); //���ļ����ڵ��ķ��ļ�
  if (!myfile.is_open()){ //������ķ�ʧ�ܣ������˳�����
		cout << "�޷����ļ�" << endl;
		return;
	}
  string nows;  //��ǰ�ַ���
  while(getline(myfile,nows)){
    if(nows[0]=='$') continue;  //��Ԫ������ע�ͣ�����
    create_NFA(nows);
  }
  showNFA();
  myfile.close();
}
struct FF{  //DFA��ת�����������ݽṹ
  char zu[MAXN]={0};  //���������ֱ���ʽ�ӵ����С���,������NFA���ļ��ϣ������ս��
  char zh;
  char yo[MAXN]={0};
};
struct K{
  char K[MAXN]={0};
};
struct DFA{ //DFA���ݽṹ
  K Kk[MAXN]={0}; //��㼯�ϵļ���
  int nk=0;  //��㼯������
  FF f[MAXN]={0}; //ת�������ļ��ϣ������ߡ�
  int nf=0; //���ߡ��ĸ���
} dfa;
void CopyS(char *a,char *b){  //��b�����ݸ��Ƶ�a�У����ߺ���
  for(int k=0;b[k];k++) a[k]=b[k];
}
void SortS(char *S){  //���ַ��������򣬲������,���ߺ���
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
bool equalS(char *S1,char *S2){ //�ж������ַ������Ƿ�ȼۣ�������ͣ����ߺ���
  int n1=0,n2=0;
  for(n1=0;S1[n1];n1++) ;
  for(n2=0;S2[n2];n2++) ;
  if(n1!=n2) return false;
  for(int i=0;i<n1;i++){
    if(S1[i]!=S2[i]) return false;
  }
  return true;
}
bool equalSS(char *A,string B){ //�����ж�һ���ַ�������һ�ַ����Ƿ�ȼۣ����ߺ���
  int nb=B.length();
  int na=0;
  for(na=0;A[na];na++) ;
  if(na!=nb) return false;
  for(int i=0;i<na;i++){
    if(A[i]!=B[i]) return false;
  }
  return true;
}
void closure(char* Sl,char *Sr){  //Slָ���Ǻ����õ����Ӽ���Srָ�������뺯�����Ӽ�����հ�
  int l=0;  //��¼Sl�е�Ԫ�ظ���
  for(int i=0;Sr[i];i++){ //�Ȱ�Sr�ڵ�Ԫ���Լ��Ž�ȥ
    Sl[l]=Sr[i];
    l++;
  }
  for(int i=0;Sl[i];i++){ //����Sl��ÿ��Ԫ�أ�����ͨ��'@'ת���ķ��ս������Sl��
    for(int j=0;j<nfa.nf;j++){  //����ÿһ����
      if((nfa.f[j].zuo==Sl[i])&&(nfa.f[j].zho=='@')){ //������һ�������ΪSl[i]���ұ�����'@'
        char temr=nfa.f[j].you; //ȡ�ߵ��ұ߽��
        bool pan=false;
        for(int k=0;Sl[k];k++){ //�жϸý���Ƿ��Ѿ���Sl��
          if(Sl[k]==temr){  //�������Ѿ���Sl�У�panȡtrue������Ĭ����false
            pan=true;
            break;
          }
          else continue;
        }
        if(pan) continue; //��������Sl�У��ж���һ����
        Sl[l]=temr; //����temr���ұߵĽ��ŵ�Sl��
        l++;
      }
      else continue;  //����ñ߲���������������,����һ����
    }
  }
  SortS(Sl);  //���и÷ŵĽ�㶼����֮����һ�����Ա�Ա������Ƿ����
}
void MoveS(char *A,char a,char *B){ //move(A,a)��Ϊ�˷�ֹײ�ؼ��ִ�д��һ�£����˸�S,B������
  int nb=0; //��¼B��Ԫ�ظ���
  for(int i=0;A[i];i++){  //����A�е�ÿ�����
    for(int j=0;j<nfa.nf;j++){  //��ÿ����㣬����nfa��ÿ����
      if((nfa.f[j].zuo==A[i])&&(nfa.f[j].zho==a)){  //����������A[i]���ս��Ϊa
        B[nb]=nfa.f[j].you; //�ѱ��Ҳ������B��
        nb++; //nb��һ
      }
      else continue;
    }
  }
  SortS(B); //����Ҫ���ȫ����֮���B����һ��
}
void getTerminator(char *S){  //ȡ��ȫ���ս���������Ӽ�������DFA�����ߺ���
  int s=0;
  for(int i=0;i<nfa.nf;i++){  //����ÿһ����
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
void create_DFA(){  //���������ɵ�NFA����DFA��ʹ���Ӽ���
  char Zero[MAXN]={'S'};  //����ͷ���
  closure(dfa.Kk[0].K,Zero); //��ͷ�ڵ�ıհ�����Ϊ��һ���Ӽ�
  dfa.nk=1; //�տ�ʼֻ��һ��dfa��㣬����dfa.K[0]
  char Ter[MAXN]={0}; //������������ս��
  getTerminator(Ter); //ȡ�������ս��
  for(int i=0;i<dfa.nk;i++){  //��һ�����ͱ���i�����桰��ǡ���i�ߵ��ĸ�λ��˵���ĸ��Ӽ�����Ǵ�����
    for(int j=0;Ter[j];j++){  //ÿ���ս������һ��
      char TemS[MAXN]={0};  //�����Ӽ�
      MoveS(dfa.Kk[i].K,Ter[j],TemS);  //��move(dfa.K[i],Ter[j])
      char PointS[MAXN]={0};  //Ŀ���Ӽ�
      closure(PointS,TemS); //��-closure(move(dfa.K[i],Ter[j]))��������PointS��
      if(!(PointS[0])) continue; //����PointS��һ���ַ�Ϊ�գ�ֱ�ӿ���һ���ս��
      bool pan=false;
      for(int t=0;t<dfa.nk;t++){  //����һ��dfa.K����PointS�Ƿ��Ѿ���dfa.K��
        if(equalS(dfa.Kk[t].K,PointS)){  //���������PointS��ͬ�ģ�panΪtrue���Ƴ�����
          pan=true;
          break;
        }
        else continue;  //�����������
      }
      if(pan){ //����pan==true��˵��PointS����K�У�ֻ���һ���߼���
        if(Ter[j]=='@') continue; //���������'@'���򲻴�
        CopyS(dfa.f[dfa.nf].zu,dfa.Kk[i].K); //f[dfa.nf]��������dfa.K[i]������Ӽ�
        dfa.f[dfa.nf].zh=Ter[j];  //f[dfa.nf]���м����Ter[j]������ս��
        CopyS(dfa.f[dfa.nf].yo,PointS); //f[dfa.nf]���Ҳ����PointS������Ӽ�
        dfa.nf++; //dfa������һ
      }
      else{ //����˵���õ����Ӽ�����K�У���Ҫ�ѽ��Ž�ȥ��������һ����
        if(Ter[j]=='@') continue; //���������'@'���򲻴�
        CopyS(dfa.f[dfa.nf].zu,dfa.Kk[i].K); //f[dfa.nf]��������dfa.K[i]������Ӽ�
        dfa.f[dfa.nf].zh=Ter[j];  //f[dfa.nf]���м����Ter[j]������ս��
        CopyS(dfa.f[dfa.nf].yo,PointS); //f[dfa.nf]���Ҳ����PointS������Ӽ�
        dfa.nf++; //dfa������һ
        CopyS(dfa.Kk[dfa.nk].K,PointS);  //�������֮���PointS������K��
        dfa.nk++; //��㣨�Ӽ�������һ
      }
    }
  }
}
void showDFA(){ //չʾ���ɵ�DFA
  cout<<endl<<"���ɵ�DFA���£�"<<endl;
  cout<<"���"<<"        "<<"�ս��"<<"        "<<"�յ�"<<endl;
  int ndfa=0;
  for(int i=0;i<dfa.nf;i++){  //����ÿһ����
    ndfa++;
    for(int j=0;j<dfa.nk;j++){
      if(equalS(dfa.Kk[j].K,dfa.f[i].zu)) cout<<j; //���������������飬��������K�Ĵ���������
      else continue;
    }
    cout<<"             "<<dfa.f[i].zh<<"            ";  //����ս��
    for(int j=0;j<dfa.nk;j++){
      if(equalS(dfa.Kk[j].K,dfa.f[i].yo)) cout<<j; //���������������飬��������K�Ĵ���������
      else continue;
    }
    if(i!=(dfa.nf-1)) cout<<endl;
  }
  cout<<endl<<"0Ϊ��ʼ���        ";
  for(int i=0;i<dfa.nk;i++){
    for(int j=0;dfa.Kk[i].K[j];j++){
      if(dfa.Kk[i].K[j]=='Z'){
        cout<<i<<' ';
        break;
      }
    }
  }
  cout<<"Ϊ�������"<<"��DFA����"<<ndfa<<"����";
}
bool is_EndNode(char *A){ //�ж�һ�Ӽ��Ƿ��ǽ�����㣬�����ͣ����ߺ���
  for(int i=0;A[i];i++){
    if(A[i]=='Z') return true;
  }
  return false;
}
string del_FirstS(string A){  //�õ�stringɾȥ��һ���ַ����ַ��������ߺ���
  if(A.length()==0) return "";  //�մ���Ȼ���ؿմ�
  else{
    string tem="";
    for(int i=1;i<A.length();i++) tem+=A[i];
    return tem;
  }
}
bool distinguish_Word(int node,string word){  //ʶ��ĳһ����word,�������Ϊnode
  if(word.length()==0)  return false; //����word�ǿմ���ֱ��return false
  else if(word.length()==1){ //����word����Ϊ1
    for(int i=0;i<dfa.nf;i++){  //����dfa��ÿһ���ߣ��ҳ����д�node�Ž���𣬱�����word[0]�ı�
      if(equalS(dfa.Kk[node].K,dfa.f[i].zu)&&dfa.f[i].zh==word[0]){ //�ҵ���
        if(is_EndNode(dfa.f[i].yo)) return true;  //����ñ��ұߵĽ���ǽ����ڵ㣬����true
        else continue;  //�����������������
      }
    } //forѭ��������û��return˵��û���ҵ����������ı�
    return false; //�Ҳ������������ıߣ�return false
  }
  else{ //word���ȴ���1
    char noww=word[0];  //��ǰҪ�жϵ��ַ�
    string Nword="";
    Nword=del_FirstS(word); //ȡNwordΪwordɾȥ��һ���ַ�
    for(int i=0;i<dfa.nf;i++){  //�ݹ����ÿһ����node�Ž�������������noww�ı�
      if(equalS(dfa.Kk[node].K,dfa.f[i].zu)&&dfa.f[i].zh==noww){  //�ҵ���
        int nex=0;  //����ñ��ұ߽���
        for(int j=0;j<dfa.nk;j++){
          if(equalS(dfa.Kk[j].K,dfa.f[i].yo)){  //�ҵ��Ӽ������뵱ǰ���Ҳ���ͬ�Ľ��
            nex=j;  //�õ�����
            break;
          }
          else continue;
        }
        if(distinguish_Word(nex,Nword)) return true;  //������һ��㣬Nword�ַ���ʶ��ɹ�
        else continue;  //������һ����
      }
      else continue;  //�߲���������������һ����
    } //forѭ������˵��û�и��ַ�������
    return false;
  }
}
string create_SonS(string A,int a,int b){  //����һ���ַ���A��aλ�õ�bλ�õ��Ӵ������ߺ���
  if(a<0) return "";
  if(b>=A.length()) return "";
  string tem="";
  for(int i=a;i<=b;i++) tem+=A[i];
  return tem;
}
string keyword[MAXN]={"main","cin","cout","auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","unsigned","union","void","volatile","while"};
bool isKeyword(string A){ //�ж��Ƿ����ڹؼ��֣�������Ǳ�ʶ�������ߺ���
  for(int i=0;i<35;i++){
    if(A==keyword[i]) return true;
    else continue;
  }
  return false;
}
char jie[MAXN]={',',';','(',')','[',']','{','}'};
bool isLimiter(char a){ //�ж��Ƿ����ڽ�������ߺ���
  for(int i=0;i<8;i++){
    if(jie[i]==a) return true;
    else continue;
  }
  return false;
}
char suan[MAXN]={'+','-','*','/','%','&','^','=','>','<'};
bool isOperator(char a){  //�ж��Ƿ���������������ߺ���
  for(int i=0;i<10;i++){
    if(suan[i]==a) return true;
    else continue;
  }
  return false;
}
string printToken(string A,string B){  //���Token��
  if(A[0]=='l'||A[0]=='e'||A[0]=='i'||A[0]=='_'){ //��ʶ����ؼ���
    if(isKeyword(B)){ //�ؼ���
      cout<<"keyword"<<"        ";
      return "keyword        ";
    }
    else{ //���ڹؼ��������ڣ�����ʶ��
      cout<<"identifier"<<"      ";
      return "identifier      ";
    }
  }
  else if(isLimiter(A[0])){ //���
    cout<<"limiter"<<"         ";
    return "limiter         ";
  }
  else if(isOperator(A[0])){  //�����
    cout<<"operator"<<"        ";
    return "operator        ";
  }
  else{ //ʣ�µ�������ͨ���ֻ��ǿ�ѧ���������Ǹ�������������
    cout<<"number"<<"          ";
    return "number          ";
  }
}
string charConnectS(char a,string b){ //��string b��ͷ������a�������µ��ַ��������ߺ���
  string c="";
  c+=a;
  for(int i=0;i<b.length();i++) c+=b[i];
  return c;
}
void distinguish_Sentence(string SentencE,int line){ //ʶ��ĳһ�д��룬��һ������,line������
  string sentence=SentencE;
  for(int i=0;i<sentence.length();i++){ //���ȶ�sentence���д���
    if(sentence[i]!='e'&&sentence[i]!='i'){ //�ѳ���i��e֮�����ĸ���l
      if((sentence[i]>='a'&&sentence[i]<='z')||(sentence[i]>='A'&&sentence[i]<='Z')) sentence[i]='l';
    }
    if(sentence[i]=='0') sentence[i]='o'; //0���Сo
    if(sentence[i]>='1'&&sentence[i]<='9') sentence[i]='z'; //��������1-9�����z
  }
  int head=0,now=0,pre=0; //����int������ָ�����ã�head��pre����ǰһ����ʶ����ַ�����head��now����ǰʶ����ַ���
  while(now<sentence.length()){ //������������ľ���
    if(create_SonS(sentence,head,now)==" "){  //�����ǿյģ�˵��head��now��ͬһλ���ҵ�ǰλ��Ϊ�յģ�����
      now++;
      head++;
      pre++;
      continue;
    }
    while(distinguish_Word(0,create_SonS(sentence,head,now))){  //ֻҪhead��now����ʶ��
      pre=now;  //��pre����now
      now++;  //nowָ����һ���ַ�
      if(sentence[now]=='.') now++; //���������������ʶ��С������ѧ�������븴��
      if((sentence[now]=='+'||sentence[now]=='-')&&(sentence[now+1]=='i'||sentence[now+1]=='z')) now++;
      if(sentence[now]=='z'&&sentence[now+1]=='i') now++;
      if((sentence[now]=='e')&&(sentence[now+1]=='z')) now++;
    } //while����˵��head��now���޷�ƥ��
    if((sentence[head]=='z'||sentence[head]=='o')&&(sentence[now]=='l'||sentence[now]=='_')){ //��������ʶ�����ֿ�ͷ�ķǷ���ʶ��
      char saveChar=SentencE[head];
      head++;
      while(distinguish_Word(0,create_SonS(sentence,head,now))){
        pre=now;
        now++;
      } //while����˵��head��now���޷�ƥ��
      cout<<line<<"        "<<"wrong"<<"          "<<saveChar<<create_SonS(SentencE,head,pre)<<endl;
      vec.insert(vec.begin()+nvec,to_string(line)); //���wrong���ѽ�����vec��
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
    vec.insert(vec.begin()+nvec,to_string(line)); //����ʶ��Ĳ��ַŽ�vec��
    nvec++;
    vec.insert(vec.begin()+nvec,printToken(create_SonS(sentence,head,pre),create_SonS(SentencE,head,pre)));
    nvec++;
    cout<<create_SonS(SentencE,head,pre)<<"\n";
    vec.insert(vec.begin()+nvec,create_SonS(SentencE,head,pre));
    nvec++;
    head=now; //ÿ�Ž�ȥһ�������¶�λһ�Σ���head��pre����now
    pre=now;
  }
}
void distinguish_File(){
  ifstream myfile;  //���ڴ�Դ�����ļ�
  myfile.open("test.txt");
  string tem;
  int line=1;
  while(getline(myfile,tem)){ //ȡ��Դ�����ÿһ��
    distinguish_Sentence(tem,line);
    tem="";
    line++; //����������++
  }
  myfile.close(); //�ر��ļ��Ա�����
}
int main(){
  load_Grammar(); //�����ķ�
  create_DFA();   //����dfa
  showDFA();  //չʾdfa����ע�͵�
  cout<<endl;
  distinguish_File(); //ʶ������ļ�
  ofstream fout;  //������token�ļ����
  fout.open("token.txt");  //���ļ�
  int tem=0;
  for(int i=0;i<vec.size();i++){
    fout<<vec[i]; //Ϊ�˷�ֹ���ݶ�ʧ����ȫ������vec�У�һ�����
    tem++;
    if(tem!=3) fout<<"        ";
    if(tem==3) fout<<"\n";  //ÿ�������ݾͻ�һ����
    if(tem==3) tem=0;
  }
  fout.close(); //�ر��Ա����ļ�
  getchar();
  return 0;
}