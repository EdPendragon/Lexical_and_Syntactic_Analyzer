#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <stack>
#include <vector>
#include <fstream>
#include <stack>
using namespace std;
#define MAXN 500  //�����������ֵ
vector<string> gra; //��������ķ��Ĳ���ʽ������
vector<string> tok; //�������token���ݵ�����
vector<char> Vt;  //����ս��������
vector<char> Vn;  //��ŷ��ս��������
vector<int> hangh;  //����кŵ�int����
void load_Grammar(){  //�����ķ�txt�ļ��������еı��ʽ����vector gra��
  ifstream myfile;  //�������ķ��ļ�
  myfile.open("grammar_2.txt");  //��2���ķ�
  if (!myfile.is_open()){ //������ķ�ʧ�ܣ������˳�����
		cout << "�޷����ķ��ļ�" << endl;
		return;
	}
  string nows="T->S";  //��ǰ�ַ�������ʼ��ΪT->S������TΪ�����µĿ�ʼ���ţ�SΪԭ����ʼ����
  gra.insert(gra.begin(),nows);
  while(getline(myfile,nows)){  //���ж�ȡ
    if(nows[0]=='$') continue;  //��Ԫ������ע�ͣ�����
    gra.insert(gra.begin()+gra.size(),nows);  //�ѵ�ǰ���ַ�������gra��
  }
  cout<<"�����2���ķ�����:"<<endl;
  if(gra.size()==0) gra.insert(gra.begin(),"@");  //@�����
  for(int i=0;i<gra.size();i++){  //չʾ�����2���ķ�����ע�͵�
    cout<<gra[i];
    cout<<endl;
  }
  myfile.close();
}
void load_Token(){  //����token�ļ����������ݱ�����vector
  ifstream myfile;  //�������ļ�
  myfile.open("token.txt");
  if (!myfile.is_open()){ //������ķ�ʧ�ܣ������˳�����
		cout << "�޷���token�ļ�" << endl;
		return;
	}
  string nows=""; //��ǰ������ַ���
  while(getline(myfile,nows)){  //���ж�ȡ
    string ha=""; //�����к�
    ha+=nows[0];  //token��ÿ�е�һ���ַ���������
    if(nows[1]>='0'&&nows[1]<='9') ha+=nows[1]; //��λ���ڶ����ַ�Ҳ����������
    int han=atoi(ha.c_str()); //stringתint
    hangh.insert(hangh.begin()+hangh.size(),han); //�����к�vector
    string Type=""; //��ǰ��token������
    string Value="";  //��ǰ��token�����ֵ
    int i=0;  //�������������ַ���
    for(i=0;i<nows.length();i++){
      if(nows[i]>='0'&&nows[i]<='9') continue;  //����i��λ����������ֵ��0��9֮�䣬����
      else if(nows[i]==' '&&Type.length()==0) continue; //�����ո�����
      else if(nows[i]==' '&&Type.length()!=0) break;  //���������ո���Type����˵������¼�����
      else Type+=nows[i]; //���������������ַ�
    }
    for(;i<nows.length();i++){  //i�нӸղŵ�λ�ò���
      if(nows[i]==' ') continue;  //�����ո�����
      else Value+=nows[i];
    }
    if(Type=="identifier") tok.insert(tok.begin()+tok.size(),"i");  //��ʶ��Ϊi
    if(Type=="number") tok.insert(tok.begin()+tok.size(),"n");  //����Ϊn��������ѧ��������
    if(Type=="keyword"){  //�ؼ���Ҫ�ֳɶ���������
      if(Value=="main") tok.insert(tok.begin()+tok.size(),"i"); //����main�����������ԣ�ʶ��Ϊi
      else if(Value=="return") tok.insert(tok.begin()+tok.size(),"r");  //returnΪr
      else if(Value=="for") tok.insert(tok.begin()+tok.size(),"f"); //forΪf
      else if(Value=="while") tok.insert(tok.begin()+tok.size(),"w"); //whileΪw
      else if(Value=="break") tok.insert(tok.begin()+tok.size(),"b"); //breakΪb
      else if(Value=="continue") tok.insert(tok.begin()+tok.size(),"c");  //continueΪc
      else if(Value=="if") tok.insert(tok.begin()+tok.size(),"z");  //ifΪz
      else if(Value=="int"||Value=="double"||Value=="float"||Value=="string"||Value=="long"||Value=="short"||Value=="char"||Value=="void") tok.insert(tok.begin()+tok.size(),"t");  //���ϴ����������ͣ����ں������Է��ذ�void���˽�ȥ
      else tok.insert(tok.begin()+tok.size(),"other_keyword");  //�����ؼ��֣�����ԭ�����ϱ�һ�£���Ӧ����ϸ���������鷳���Ҳ���
    }
    if(Type=="operator"){ //�����ҲҪ����
      if(Value=="=") tok.insert(tok.begin()+tok.size(),"=");  //=�����ó���
      else if(Value=="<="||Value=="=="||Value==">="||Value==">"||Value=="<"||Value=="!=") tok.insert(tok.begin()+tok.size(),"p");  //�����ж����õ����������p
      else tok.insert(tok.begin()+tok.size(),"o");  //�����ֱ������p
    }
    if(Type=="limiter") tok.insert(tok.begin()+tok.size(),Value); //���ֱ�������Լ�
  }
  cout<<"�����tokenת��Ϊ�ս��������:"<<endl; //չʾ���̣���ע�͵�
  for(int j=0;j<tok.size();j++) cout<<tok[j];
  cout<<endl;
}
string simpli(string str){  //���򣬼�ɾȥ�ַ������ظ��ַ������ߺ���
  string t="";
  for(int i=0;i<str.length();i++){
    bool pan=false;
    for(int j=0;j<i;j++){
      if(str[j]==str[i]){
        pan=true;
        break;
      }
      else continue;
    }
    if(pan) continue; //panΪtrue˵��ǰ�����ظ���
    else t+=str[i]; //�����str[i]����t��
  }
  return t;
}
string SortS(string s){ //�����ַ�����С�����ţ����ߺ���
  string t=s;
  for(int i=0;i<(t.length()-1);i++){  //ð�����򣬿ɸĳ�Ч�ʸ��ߵ��㷨
    for(int j=i+1;j<t.length();j++){
      if(t[i]<t[j]) continue;
      else{
        char tem=t[j];
        t[j]=t[i];
        t[i]=tem;
      }
    }
  }
  return t;
}
string rearS(string s){ //ȥ��s��ͷ����һ���ַ�������ʣ����ַ��������ߺ���
  string t="";
  for(int i=1;i<s.length();i++) t+=s[i];
  return t;
}
string getFirst(char X){  //ȡ��First����,���ڿ����ж����ÿ��ֻռ��һ��char��������string����
  string t="";
  if(X<'A'||X>'Z'){ //�����ս����ͷ����a��������ôFirst���������Լ�����a
    t+=X;
    return t;
  }
  else{ //����˵�����ս����ͷ
    for(int i=0;i<gra.size();i++){  //����ÿһ�����ʽ
      if(gra[i][0]==X){ //ֻ��X->���� �Żῼ��
        if(gra[i][3]==X) continue;  //����X->X����֮�������������һ���ʽ
        if(gra[i][3]>='A'&&gra[i][3]<='Z'){ //X->Y������������ս��������1��2�Ǽ�ͷ�����ж�3
          string tem=getFirst(gra[i][3]); //��First[Y]
          for(int i=0;i<tem.length();i++){
            if(tem[i]!='@') t+=tem[i]; //ȥ��Y�еĿ�
          }
          bool pan=false; //�ж�First[Y]���Ƿ��п�
          for(int i=0;i<tem.length();i++){
            if(tem[i]=='@'){  //�ҵ��գ�panΪtrue
              pan=true;
              break;
            }
            else continue;
          }
          if(pan){  //ֻ�е�Y��First�����пյ�ʱ��ſ������¹���
            if(gra[i].length()==4){ //��X�C>Y���� �� ����First(X)��
              t+='@';
              continue;
            }
            else if(gra[i][4]>='A'&&gra[i][4]<='Z'){  //��X�C>YZ����������First(Z)����First(X)��
              string temp="";
              temp=getFirst(gra[i][4]);
              for(int k=0;k<temp.length();k++) t+=temp[k];
            }
            else{ //��X�C>Ya���������� a ����First(X)��
              t+=gra[i][4];
            }
          }
          else continue;  //������һ�����ʽ
        }
        else{ //X->a����,����ս��
          t+=gra[i][3];  //���в���ʽX->a��������a����t��
          continue;
        }
      }
      else continue;  //��������
    }
    t=simpli(t);  //ȥ��t���ظ����ַ�
    t=SortS(t); //����˳���ź�
    return t;
  }
}
string getSFirst(string s){ //ȡ���ַ���s��first������aABC��ABa
  string t="";
  if(s.length()==0){  //�մ�ֱ�ӷ��ؿ�
    t+='@';
  }
  else{ //������������
    if(s[0]>='A'&&s[0]<='Z'){ //�ַ����׸��ַ��Ƿ��ս��
      string tem1=getFirst(s[0]);  //s[0]��first��
      string tem2=getSFirst(rearS(s)); //sȥͷ��first��
      bool pan=false; //�ж�s[0]��first������û�п�
      for(int i=0;i<tem1.length();i++){
        if(tem1[i]!='@') t+=tem1[i];
        else pan=true;
      }
      if(pan){  //����panΪtrue��˵��s[0]��first������@
        for(int i=0;i<tem2.length();i++) t+=tem2[i];  //��ȥͷ��s��first���Ž�ȥ
      }
    }
    else{ //����˵���ַ����׸��ַ����ս��
      t+=s[0];  //ֱ�ӷ��ظ��ս��
    }
  }
  t=simpli(t);
  t=SortS(t);
  return t;
}
void showFirst(){ //չʾ���з��ս�����ս����First��
  for(int i=0;i<gra.size();i++){  //����ÿһ���ķ�����ʽ
    char t=gra[i][0];
    bool pan=false; //�ж�t�Ƿ��Ѿ���Vn�У�֮���Բ���Vt����Ϊ��ͷ��߱��Ƿ��ս��
    for(int j=0;j<Vn.size();j++){
      if(Vn[j]==t){
        pan=true;
        break;
      }
      else continue;
    }
    if(pan==false) Vn.insert(Vn.begin()+Vn.size(),t); //t����Vn����Ž�ȥ
    for(int j=3;j<gra[i].length();j++){ //��3��ʼ����Ϊ0�ѿ��ǹ���1��2�Ǽ�ͷ
      t=gra[i][j];
      if(t>='A'&&t<='Z'){ //���ս��
        pan=false; //�ж�t�Ƿ��Ѿ���Vn�У�֮���Բ���Vt����Ϊ��ͷ��߱��Ƿ��ս��
        for(int k=0;k<Vn.size();k++){
          if(Vn[k]==t){
            pan=true;
            break;
          }
          else continue;
        }
        if(pan==false) Vn.insert(Vn.begin()+Vn.size(),t); //t����Vn����Ž�ȥ
      }
      else{ //�ս��
        pan=false; //�ж�t�Ƿ��Ѿ���Vn�У�֮���Բ���Vt����Ϊ��ͷ��߱��Ƿ��ս��
        for(int k=0;k<Vt.size();k++){
          if(Vt[k]==t){
            pan=true;
            break;
          }
          else continue;
        }
        if(pan==false) Vt.insert(Vt.begin()+Vt.size(),t); //t����Vn����Ž�ȥ
      }
    }
  }
  cout<<"����չʾ���з��ս����First��"<<endl;
  for(int i=0;i<Vn.size();i++){
    string t=getFirst(Vn[i]);
    cout<<Vn[i]<<"        ";
    for(int j=0;j<t.length();j++) cout<<t[j]<<"  ";
    cout<<endl;
  }
}
vector<vector<string>> I; //���������Ŀ����
vector<string> ii;  //������I�д����Ŀ��,��������
struct Edge{
  int zuo;  //��ʼ��Ŀ���ı��
  char zhong; //���ϵķ��ս�����ս��
  int yo; //��ͷָ����Ŀ���ı��
} bian[MAXN]={0}; //���������Ŀ��֮��ıߵļ���
int ne=0; //����Ŀǰ�Ѵ�ıߵĸ���
int has_existedi(vector<string> iii){ //�ж�iii�Ƿ��Ѿ���I��
  if(I.size()==0) return -1;  //����IΪ�գ�ֱ�ӷ���-1
  int t=-1; //Ĭ��ֵΪ-1
  for(int i=0;i<I.size();i++){  //������Ŀ����I�е�ÿһ����Ŀ��
    int ni=iii.size(),nI=I[i].size();  //niΪiii�ı��ʽ������nIΪI[i]�ı��ʽ����
    if(ni!=nI) continue;  //������߱��ʽ������ͬ��ֱ���ж���һ����Ŀ��
    bool pan=true; //pan��������Ƿ���ȫ���
    for(int j=0;j<ni;j++){  //����ÿһ�����ʽ
      if(iii[j]!=I[i][j]){ //���������ĳһ�����ʽ����
        pan=false;  //pan�ĳ�false
        break;  //�˳�ѭ��
      }
    }
    if(pan){  //���������ȫ���
      t=i;  //����i
      break;
    }
    else continue;
  }
  return t; //����һֱû�ҵ���ii��ȫ��ͬ����Ŀ����t����Ĭ��ֵ-1
}
bool xian(string a,string b){ //�ж������ַ������Ⱥ�˳����������Ŀ������,true˵��a��ǰ��
  int na=a.length(),nb=b.length();
  if(na<nb) return true;  //a�̣�����true
  else if(na>nb) return false;  //a��������false
  else{ //�������һ����
    for(int i=0;i<na;i++){
      if(a[i]<b[i]) return true;  //ĳһλa���ַ���b��С������true
      else if(a[i]>b[i]) return false;  //ĳһλb���ַ���aС������false
      else continue;  //����˵��������ȣ���������һ���ַ�
    }
    return false; //������ȫһ�£�����false
  }
}
vector<string> Sorti(vector<string> iiii){  //��iiii�еı��ʽ���մ�С��������
  vector<string> iii;
  for(int i=0;i<iiii.size();i++) iii.insert(iii.begin()+iii.size(),iiii[i]);
  for(int i=0;i<(iii.size()-1);i++){ //�ܼ򵥵�ð������
    for(int j=i+1;j<iii.size();j++){ //�����ַ������ַ��������Ϳ�����<�Աȣ����Բ���̫���ӵĲ���
      if(xian(iii[i],iii[j])) continue; //����ǰ�ı��ʽ��ĸ˳��С���ö�
      else{ //����򵥵Ľ���
        string tem=iii[j];
        iii[j]=iii[i];
        iii[i]=tem;
      }
    }
  }
  return iii;
}
vector<string> simplii(vector<string> iiii){  //ȥ����Ŀ��iiii�ж���ı��ʽ
  vector<string> iii;
  for(int i=0;i<iiii.size();i++) iii.insert(iii.begin()+iii.size(),iiii[i]);
  iii=Sorti(iii); //��������iii
  vector<string> ti;  //���������Ĺ��������������治�ظ���ii�б��ʽ
  for(int i=0;i<iii.size();i++){ //����ii�е�ÿ�����ʽ�����Ƿ�����ti��
    bool pan=false; //pan����true˵��ti�����к�ii[i]��ȵı��ʽ
    for(int j=0;j<ti.size();j++){ //����ti���Ѵ��ڵ�ÿһ�����ʽ
      if(ti[j]==iii[i]){ //����ti���к�ii[i]��ȵı��ʽ��pan�ĳ�true���˳�ѭ��
        pan=true;
        break;
      }
      else continue;  //���������ti��ʣ���
    }
    if(pan) continue; //����pan����true�ǾͲ���ti�����
    else{ //����Ž�ȥ
      ti.insert(ti.begin()+ti.size(),iii[i]);
    }
  } //forѭ������˵���Ѿ��Ѹ÷ŵ�ȫ������
  iii.clear(); //iii��գ�����������
  for(int i=0;i<ti.size();i++) iii.insert(iii.begin()+iii.size(),ti[i]); //�������ȥ
  iii=Sorti(iii); //�Է���һ����������
  return iii;
}
char afterNode(string s){ //���һ�����ʽ���ߵ��ַ������ߺ���
  for(int i=0;i<s.length();i++){
    if(s[i]=='`') return s[i+1];
    else continue;
  }
  return 'a'; //��ֹ�����һ������ֵ����ʵ�ò����������Ҳ���������������û��ı��ʽ��
}
vector<char> getdianh(vector<string> nows){ //ȡ�õ�ǰ��Ŀ���е������з���
  vector<char> t; //����ֵ
  for(int i=0;i<nows.size();i++){
    char tem=afterNode(nows[i]);    //�õ�nows��ÿһ�����ʽ���ߵ��ַ�
    if(tem=='$') continue;//������߾��Ƕ��ţ�������ֱ������
    t.insert(t.begin()+t.size(),tem); //����
  }
  return t;
}
vector<string> getClosure(string nows){ //��ÿ���հ�����Ŀ��,��˳����������Ŀ�����ӵı�
  ii.clear(); //���Ƚ�ii��գ����������ӹ���������ְ��Ц��
  ii.insert(ii.begin()+ii.size(),nows); //���Ƚ�nows�Ž�ȥ,ii.size��ʵ��0��дϰ�߲������
  for(int i=0;i<ii.size();i++){ //�����Ѿ����ȥ��ÿһ�����ʽ,���ڽ�����ж���,���Խ����ʽ�еĶ�����$��ʾ
    int dian=0,dou=0; //�������ʽ�С�(`)��λ�ã�dou������ʽ�ж��ţ�$����λ��
    for(int j=0;j<ii[i].length();j++){  //��ȡ��dian��dou��λ��
      if(ii[i][j]=='`') dian=j;
      if(ii[i][j]=='$') dou=j;
    }
    if(ii[i][dian+1]=='$') continue;  //������߾��Ƕ��ţ�����S->S��,#  ���������µĲ���ʽ��ֱ�ӿ���һ������ʽ
    if((ii[i][dian+1]<'A')||(ii[i][dian+1]>'Z')) continue;  //����������ս��������B->��aB,a/b  ��Ҳ�������µĲ���ʽ��ֱ�ӿ���һ������ʽ
    else{ //����˵�������Ƿ��ս����Ҫ�����ķ����ʽ�����µ�ʽ��
      string newr=""; //newr���������ɵı��ʽ����($)��ߵ�����
      if(ii[i][dian+2]=='$'){ //��������λֱ�Ӿ��Ƕ��ţ���S->��S,# ����ʱnewr����#
        for(int j=dou+1;j<ii[i].length();j++) newr+=ii[i][j];
      }
      else{ //����˵�������λ���Ƕ��ţ���S->��BB,#����ʱnewr����first(B#)
        string tem="";
        for(int j=dian+2;j<dou;j++) tem+=ii[i][j];
        tem=getSFirst(tem);
        bool pan=false; //panΪtrue����first(tem)���п�
        for(int j=0;j<tem.length();j++){
          if(tem[j]!='@') newr+=tem[j]; //�Ѳ�Ϊ�յķŽ�ȥ
          else pan=true;  //�����пգ����Ȳ��Ž�ȥ��pan�ĳ�true
        }
        if(pan){  //panΪtrue˵���п�
          for(int j=dou+1;j<ii[i].length();j++) newr+=ii[i][j]; //�Ѷ��ź�ߵķŽ�ȥ
        }
      }
      char nc=ii[i][dian+1];  //��nc��ŵ��ߵĵ�һ���ַ�
      for(int j=0;j<gra.size();j++){  //���������ķ��е�ÿһ�����ʽ������û�м�ͷ�����nc��
        if(gra[j][0]!=nc) continue; //����gra[j]�ĵ�һ���ַ�������nc������������һ�����ʽ
        else{ //�����м�ͷ�����nc�ģ���Ҫ������Ŀ��
          string temt="";
          for(int k=0;k<gra[j].length();k++){
            if(k==3) temt+='`'; //�ڼ�ͷ��߼���һ����
            temt+=gra[j][k];
          }
          temt+='$';  //ĩβ���϶���
          for(int k=0;k<newr.length();k++) temt+=newr[k]; //��newr����ĩβ
          ii.insert(ii.begin()+ii.size(),temt); //���µı��ʽ������Ŀ��
        }
      }
    }
  } //��forѭ������˵���Ѿ�û�п��ԷŽ�ȥ�ı��ʽ�ˣ���ʱii������
  return ii;
}
bool has_existed_edge(int l,char m,int r){  //�ж�ĳһ�����Ƿ����ڱ߼�����
  for(int i=0;i<ne;i++){  //���������Ѵ��ڵı�
    if(bian[i].zuo==l&&bian[i].zhong==m&&bian[i].yo==r) return true;  //������һ�µģ�����true
    else continue;
  }
  return false; //��û��һ�µ��򷵻�false
}
void putI(vector<string> nowi){ //����Ŀ��nowi������Ŀ����I�У�����������Ӧ�ı�
  int ni=has_existedi(nowi);  //nowi�Ƿ�����I�У�����Ϊ-1����Ϊ��Ӧ���
  if(ni==-1){ //nowi���粢����I��
    ni=I.size();  //��ni��-1�ĳɶ�Ӧ���
    I.insert(I.begin()+I.size(),nowi);  //��nowi��Ӧ����Ŀ���Ž�ȥ
  }
  vector<char> nodec=getdianh(nowi);  //ȡ��ÿһ�����ʽ���ߵ��ַ�
  for(int i=0;i<nodec.size();i++){  //ÿ���ַ�����һ���µıհ���Ŀ�����������һ����
    vector<string> iii; //������ʱ�������ɵ���Ŀ������Ϊһ�����ſ��ܶ�Ӧ������ʽ��Ҫ����Щ���ʽ���ɵ���Ŀ�����ϵ�һ������ȥ��������ʽ
    for(int j=0;j<nowi.size();j++){ //����ÿһ�����ʽ
      char h=afterNode(nowi[j]);  //ȡ�õ�ǰ���ʽ���ߵ��ַ�
      if(h!=nodec[i]) continue; //���統ǰ���ʽ��ߵ��ַ������ڵ�ǰ�����ַ������������ж���һ�����ʽ
      string ts=nowi[j];  //��ts���棬nowi[j]�ĵ�͵���һ���ַ���������ַ���(�ܵ���һ��˵�����߾���nodec[i])
      for(int l=0;l<ts.length();l++){
        if(ts[l]=='`'){ //����ts�е�ǰ�ַ����ڵ�
          char toolc=ts[l]; //����
          ts[l]=ts[l+1];
          ts[l+1]=toolc;
          break;  //�������˱����˽���ѭ��
        }
      } //����ѭ��ʱts�Ѿ��������
      vector<string> zani=getClosure(ts); //���ɱհ���Ŀ��
      for(int l=0;l<zani.size();l++) iii.insert(iii.begin()+iii.size(),zani[l]);  //��zani�е�ÿ�����ʽ����iii��
    }
    iii=Sorti(iii); //iii����ȥ��������ʽ
    iii=simplii(iii);
    int niii=has_existedi(iii); //niii����iii��I��λ�ã�����I����Ϊ-1
    if(niii==-1){ //����iii����I��
      niii=I.size();  //��niiiΪ��ǰI����Ŀ����
      I.insert(I.begin()+I.size(),iii); //����iii�Ž�ȥ
    }
    if(!has_existed_edge(ni,nodec[i],niii)){  //�������ڱ߼�����û��������
      bian[ne].zuo=ni;  //����һ���ߣ����Ϊni
      bian[ne].zhong=nodec[i];  //�м�Ϊ��ǰ�ַ�
      bian[ne].yo=niii; //�ұ�Ϊniii
      ne++; //�����˰ѱ�����һ
    }
  }
}
string AG(int z,char c){  //������ACTION��GOTO�ĺ�������������ֱ�Ϊ״̬��źͶ���
  if(c>='A'&&c<='Z'){ //����c�Ƿ��ս������GOTO
    for(int i=0;i<ne;i++){
      if(bian[i].zuo==z&&bian[i].zhong==c){ //�ҵ���Ӧ�ı�
        int t=bian[i].yo;
        string tt="";
        tt+=to_string(t);
        return tt;
      }
    }
  }
  else{ //����c���ս������ACTION
    for(int i=0;i<ne;i++){  //�����ж���û�д�Iz�������м���c�ı�
      if(bian[i].zuo==z&&bian[i].zhong==c){ //�ҵ���
        string S="S";
        int Sy=bian[i].yo;
        S+=to_string(Sy);
        return S; //����Si
      }
      else continue;    //�����������һ����
    } //forѭ��������ûreturn˵�������ƽ�
    for(int i=0;i<I[z].size();i++){ //����Iz��ÿһ�����ʽ
      int dian=0; //��¼���λ��
      for(int j=0;j<I[z][i].length();j++){
        if(I[z][i][j]=='`'){
          dian=j; //��¼���λ��
          break;
        }
        else continue;  //�����������һ��
      } //forѭ��������ʱ��dian�Ѿ���¼��λ����
      if(I[z][i][dian+1]=='$'){ //ֻ�е������Ƕ��ŵĲſ���
        string rS;
        if(I[z][i][dian-1]=='S'){ //�����ǰ�����S������acc
          rS="acc";
        }
        else{ //���򷵻�rl
          rS="r";
          string toolS="";
          for(int l=0;l<dian;l++){  //��toolS��ŵ�ǰ���ʽ��ǰ��Ĳ���
            toolS+=I[z][i][l];
          }
          for(int l=0;l<gra.size();l++){
            if(toolS==gra[l]){
              rS+=to_string(l);
              break;
            }
          }
        }
        for(int l=dian+2;l<I[z][i].length();l++){ //dian+2�Ƕ��ź��һ�����ţ�����û�к�c��ͬ��
          if(I[z][i][l]==c){
            return rS;
          }
          else continue;
        }
      }
      else continue;  //������һ�����ʽ
    }
  }
  string wr="   ";  //����wrong
  return wr;
}
int wei(int A){ //��a��λ�������ߺ���
  if(A==0) return 1;
  int a=A,w=0;
  while(a>0){
    w++;
    a/=10;
  }
  return w;
}
void showFXB(){ //չʾ������
  cout<<"����ΪLR(1)������,�����ս����λACTION��,���ս����λGOTO��";
  cout<<endl<<"____"; //����ǰ�ĺ���
  for(int i=0;i<Vt.size();i++) cout<<"______";
  for(int i=0;i<Vn.size();i++) cout<<"_____";
  cout<<endl<<"I  |"; //��ͷ
  for(int i=0;i<Vt.size();i++) cout<<"  "<<Vt[i]<<"  |";  //�ս���ͷ��ս��
  cout<<"  "<<'#'<<"  |";
  for(int i=1;i<Vn.size();i++){
    cout<<"  "<<Vn[i]<<" |";
  }
  cout<<endl<<"______"; //����ǰ�ĺ���
  for(int i=0;i<Vt.size();i++) cout<<"______";
  for(int i=0;i<Vn.size();i++) cout<<"_____";
  cout<<endl;
  for(int i=0;i<I.size();i++){  //ÿ��״̬��ӡһ��
    cout<<i;
    if(wei(i)==1) cout<<"  "; //���������λ�����Լ���״̬��1λ�Ͳ���λ
    else if(wei(i)==2) cout<<" "; //����1λ
    cout<<"|";
    for(int j=0;j<Vt.size();j++){
      cout<<" "<<AG(i,Vt[j]);
      if(AG(i,Vt[j]).length()==3) cout<<" ";
      else if(AG(i,Vt[j]).length()==2) cout<<"  ";
      else if(AG(i,Vt[j]).length()==4) cout<<"";
      else if(AG(i,Vt[j])=="   ") cout<<" ";  //��ACTION�������ͳһ���λ5λ
      cout<<"|"; //�����ָ�
    }
    cout<<" "<<AG(i,'#'); //����Vt��û��#��������Ҫ�������һ��
    if(AG(i,'#').length()==3) cout<<" ";
    else if(AG(i,'#').length()==2) cout<<"  ";
    else if(AG(i,'#').length()==4) cout<<"";
    else if(AG(i,'#')=="   ") cout<<" ";  //��ACTION�������ͳһ���λ5λ
    cout<<"|"; //�����ָ�
    for(int j=1;j<Vn.size();j++){ //����Vn�ĵ�һ��Ԫ����T���ǿ�ʼ���ţ�����j��1��ʼ������0
      cout<<" "<<AG(i,Vn[j]);
      if(AG(i,Vn[j]).length()==1) cout<<"  ";
      else if(AG(i,Vn[j]).length()==2) cout<<" ";
      else if(AG(i,Vn[j]).length()==3) cout<<"";
      else if(AG(i,Vn[j])=="   ") cout<<"";
      cout<<"|";
    }
    cout<<endl;
  }
  cout<<"____";
  for(int i=0;i<Vt.size();i++) cout<<"______";
  for(int i=0;i<Vn.size();i++) cout<<"_____";  //��β
  cout<<endl; //ת����һ�У�����һ������������token
}
void AnalyS(){  //��������
  string tokenS="";
  for(int i=0;i<tok.size();i++) tokenS+=tok[i]; //��tok����������ȡ����
  tokenS+="#";  //���봮ĩβ����#
  int bu=0; //�������裬��ʼΪ0
  cout<<"����"<<" "<<"״̬ջ"<<' '<<" ����ջ   ���봮";
  for(int i=0;i<tokenS.length();i++) cout<<" ";
  cout<<"        ����";
  vector<int> Sta; //״̬ջ
  Sta.insert(Sta.begin(),0);  //״̬ջ��ʼ��Ϊ{0}
  vector<char> Symb; //����ջ
  Symb.insert(Symb.begin(),'#'); //����ջ��ʼ��Ϊ{#}
  cout<<endl;
  while(bu<100000){ //��ʵ����дwhile(1),Ϊ��ֹ�ķ���������ѭ��������һ����������
    bu++; //��������һ
    cout<<bu<<"    ";
    if(wei(bu)==1) cout<<"  "; //Ϊ������
    else if(wei(bu)==2) cout<<" ";
    for(int l=0;l<Sta.size();l++) cout<<Sta[l]<<' ';
    cout<<"    ";
    for(int l=0;l<Symb.size();l++) cout<<Symb[l]<<' ';
    cout<<"    "<<tokenS<<"                ";
    int zhuang=Sta[Sta.size()-1]; //ȡ״̬ջջ�����洢��zhuang��
    char shu=tokenS[0]; //ȡ���봮ͷ���洢��shu��
    string dang=AG(zhuang,shu); //��Ӧ���е�ֵ
    if(dang=="acc"){  //��Ϊacc������ɹ�
      cout<<"���ˣ�ţ��"<<endl<<"SUCCESS!!!!!";
      return;
    }
    else if(dang=="   "){ //����ʧ�ܣ������ķ����﷨���󣬻�����@
      string Aj=AG(zhuang,'@');
      if(Aj=="   "){
        cout<<"����"<<endl<<"FAILURE";  //���粻������@����������
        string tokenSS="";
        for(int h=0;h<tok.size();h++) tokenSS+=tok[h]; //��tok����������ȡ����
        int chu=0;  //��¼���봮�г�����ų��ִ���
        for(int h=0;h<tokenSS.length();h++){
          if(Symb[Symb.size()-1]==tokenSS[h]) chu++;
        }
        for(int h=0;h<tokenSS.length();h++){
          if(Symb[Symb.size()-1]==tokenSS[h]){
            if(chu==1){
              cout<<endl<<"�����к���"<<hangh[h]<<"�У�����";
              break;
            }
            else chu--;
          }
        }
        return;
      }
      else{
        string Sj="";
        for(int l=1;l<Aj.length();l++) Sj+=Aj[l];
        int sj=atoi(Sj.c_str());  //stringתint
        Sta.insert(Sta.begin()+Sta.size(),sj);
        Symb.insert(Symb.begin()+Symb.size(),'@');
        cout<<"��Ҫ�ÿշ�������Լ�������״̬ջѹ��"<<sj<<"���ڷ���ջ��ѹ��@";
      }
    }
    else if(dang[0]=='S'){  //�ƽ�
      string Sj="";
      for(int l=1;l<dang.length();l++) Sj+=dang[l]; //ȡ��S��ߵĲ��֣�����ת��int
      int sj=atoi(Sj.c_str());  //stringתint
      Sta.insert(Sta.begin()+Sta.size(),sj); //sj��״̬ջ
      Symb.insert(Symb.begin()+Symb.size(),shu); //��ͷ�����ջ
      tokenS=rearS(tokenS); //���봮ȥ����ͷ
      cout<<"ACTION("<<zhuang<<','<<shu<<")="<<dang<<"���ƽ�����";
    }
    else{ //��Լ,���ַ���r
      string rj="";
      for(int l=1;l<dang.length();l++) rj+=dang[l]; //ȡ��r��ߵĲ��֣�����ת��int
      int nr=atoi(rj.c_str());  //stringתint
      char lef=gra[nr][0];  //rj���ʽ��
      string rig="";  //rj���ʽ�Ҳ�
      for(int l=3;l<gra[nr].length();l++) rig+=gra[nr][l];  //ȡ��ͷ�ұߵĲ���
      int nrig=rig.length();
      for(int l=0;l<nrig;l++) Symb.pop_back();
      Symb.insert(Symb.begin()+Symb.size(),lef); //���й�Լ
      for(int l=0;l<nrig;l++) Sta.pop_back();  //״̬ջ��ջ
      int kkk=Sta[Sta.size()-1];  //ȡ״̬ջջ��
      string GOTOkX=AG(kkk,lef);  //��GOTO
      int gotokx=atoi(GOTOkX.c_str());  //תint
      Sta.insert(Sta.begin()+Sta.size(),gotokx); //GOTO��ջ
      cout<<"ACTION("<<zhuang<<','<<shu<<")="<<dang<<"����Լ����";
    }
    cout<<endl;
  }
}
int main(){
  load_Grammar(); //����2���ķ�
  load_Token(); //����token
  showFirst();  //չʾÿ�����ս����First��
  string t="T->`S$#"; //TΪ����ӵĿ�ʼ���ս�����൱��S'��`����㣬$������
  vector<string> ti=getClosure(t);  //������t�ıհ���Ŀ��
  putI(ti); //�Ƚ�ti����I�У������������������µ���Ŀ������
  for(int i=0;i<I.size();i++) putI(I[i]); //���I�е�ÿ����Ŀ��������ͬ����
  cout<<"LR(1)��Ŀ��֮��Ĺ�ϵ����"<<endl;
  for(int i=0;i<ne;i++){
    cout<<bian[i].zuo<<"        "<<bian[i].zhong<<"        "<<bian[i].yo<<endl;
  }
  cout<<"ÿ����Ŀ������ı��ʽ���£�"<<endl;
  for(int i=0;i<I.size();i++){
    cout<<"I"<<i<<":"<<endl;
    for(int j=0;j<I[i].size();j++){
      for(int l=0;l<I[i][j].length();l++){
        if(I[i][j][l]=='`') cout<<"��";  //��`��ص�
        else if(I[i][j][l]=='$') cout<<","; //��$��ض���
        else cout<<I[i][j][l];
      }
      cout<<endl;
    }
    cout<<endl;
  }
  showFXB();  //չʾLR(1)������
  AnalyS(); //�������봮
  getchar();
  return 0;
}