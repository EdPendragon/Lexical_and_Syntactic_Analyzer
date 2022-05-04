#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <stack>
#include <vector>
#include <fstream>
#include <stack>
using namespace std;
#define MAXN 500  //定义数组最大值
vector<string> gra; //用来存放文法的产生式的容器
vector<string> tok; //用来存放token内容的容器
vector<char> Vt;  //存放终结符的容器
vector<char> Vn;  //存放非终结符的容器
vector<int> hangh;  //存放行号的int容器
void load_Grammar(){  //加载文法txt文件并将其中的表达式放入vector gra中
  ifstream myfile;  //用来打开文法文件
  myfile.open("grammar_2.txt");  //打开2型文法
  if (!myfile.is_open()){ //假如打开文法失败，报错并退出函数
		cout << "无法打开文法文件" << endl;
		return;
	}
  string nows="T->S";  //当前字符串，初始化为T->S，其中T为引入新的开始符号，S为原本开始符号
  gra.insert(gra.begin(),nows);
  while(getline(myfile,nows)){  //按行读取
    if(nows[0]=='$') continue;  //美元符代表注释，跳过
    gra.insert(gra.begin()+gra.size(),nows);  //把当前的字符串放入gra中
  }
  cout<<"读入的2型文法如下:"<<endl;
  if(gra.size()==0) gra.insert(gra.begin(),"@");  //@代表空
  for(int i=0;i<gra.size();i++){  //展示读入的2型文法，可注释掉
    cout<<gra[i];
    cout<<endl;
  }
  myfile.close();
}
void load_Token(){  //加载token文件，并将内容保存在vector
  ifstream myfile;  //用来打开文件
  myfile.open("token.txt");
  if (!myfile.is_open()){ //假如打开文法失败，报错并退出函数
		cout << "无法打开token文件" << endl;
		return;
	}
  string nows=""; //当前读入的字符串
  while(getline(myfile,nows)){  //按行读取
    string ha=""; //代表行号
    ha+=nows[0];  //token表每行第一个字符就是数字
    if(nows[1]>='0'&&nows[1]<='9') ha+=nows[1]; //二位数第二个字符也可能是数字
    int han=atoi(ha.c_str()); //string转int
    hangh.insert(hangh.begin()+hangh.size(),han); //放入行号vector
    string Type=""; //当前行token的类型
    string Value="";  //当前行token具体的值
    int i=0;  //用来遍历整行字符串
    for(i=0;i<nows.length();i++){
      if(nows[i]>='0'&&nows[i]<='9') continue;  //首先i的位置是行数，值在0到9之间，跳过
      else if(nows[i]==' '&&Type.length()==0) continue; //遇到空格跳过
      else if(nows[i]==' '&&Type.length()!=0) break;  //假如遇到空格且Type不空说明类型录入完毕
      else Type+=nows[i]; //遇到了类型所在字符
    }
    for(;i<nows.length();i++){  //i承接刚才的位置不变
      if(nows[i]==' ') continue;  //遇到空格，跳过
      else Value+=nows[i];
    }
    if(Type=="identifier") tok.insert(tok.begin()+tok.size(),"i");  //标识符为i
    if(Type=="number") tok.insert(tok.begin()+tok.size(),"n");  //数字为n，包括科学计数法等
    if(Type=="keyword"){  //关键字要分成多种来处理
      if(Value=="main") tok.insert(tok.begin()+tok.size(),"i"); //由于main函数的特殊性，识别为i
      else if(Value=="return") tok.insert(tok.begin()+tok.size(),"r");  //return为r
      else if(Value=="for") tok.insert(tok.begin()+tok.size(),"f"); //for为f
      else if(Value=="while") tok.insert(tok.begin()+tok.size(),"w"); //while为w
      else if(Value=="break") tok.insert(tok.begin()+tok.size(),"b"); //break为b
      else if(Value=="continue") tok.insert(tok.begin()+tok.size(),"c");  //continue为c
      else if(Value=="if") tok.insert(tok.begin()+tok.size(),"z");  //if为z
      else if(Value=="int"||Value=="double"||Value=="float"||Value=="string"||Value=="long"||Value=="short"||Value=="char"||Value=="void") tok.insert(tok.begin()+tok.size(),"t");  //以上代表数据类型，由于函数可以返回把void加了进去
      else tok.insert(tok.begin()+tok.size(),"other_keyword");  //其它关键字，分类原理与上边一致，本应继续细化，过于麻烦暂且不表
    }
    if(Type=="operator"){ //运算符也要分类
      if(Value=="=") tok.insert(tok.begin()+tok.size(),"=");  //=单独拿出来
      else if(Value=="<="||Value=="=="||Value==">="||Value==">"||Value=="<"||Value=="!=") tok.insert(tok.begin()+tok.size(),"p");  //带有判断作用的运算符试作p
      else tok.insert(tok.begin()+tok.size(),"o");  //其余的直接视作p
    }
    if(Type=="limiter") tok.insert(tok.begin()+tok.size(),Value); //界符直接试作自己
  }
  cout<<"读入的token转化为终结符串如下:"<<endl; //展示过程，可注释掉
  for(int j=0;j<tok.size();j++) cout<<tok[j];
  cout<<endl;
}
string simpli(string str){  //化简，即删去字符串中重复字符，工具函数
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
    if(pan) continue; //pan为true说明前面有重复的
    else t+=str[i]; //否则把str[i]放入t中
  }
  return t;
}
string SortS(string s){ //排序，字符串从小到大排，工具函数
  string t=s;
  for(int i=0;i<(t.length()-1);i++){  //冒泡排序，可改成效率更高的算法
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
string rearS(string s){ //去掉s的头部第一个字符，返回剩余的字符串，工具函数
  string t="";
  for(int i=1;i<s.length();i++) t+=s[i];
  return t;
}
string getFirst(char X){  //取得First集合,由于可能有多个且每个只占用一个char，所以用string返回
  string t="";
  if(X<'A'||X>'Z'){ //假如终结符开头，即a……，那么First集就是它自己，即a
    t+=X;
    return t;
  }
  else{ //否则说明非终结符开头
    for(int i=0;i<gra.size();i++){  //遍历每一个表达式
      if(gra[i][0]==X){ //只有X->…… 才会考虑
        if(gra[i][3]==X) continue;  //假如X->X……之类的则跳过到下一表达式
        if(gra[i][3]>='A'&&gra[i][3]<='Z'){ //X->Y……，后跟非终结符，由于1与2是箭头所以判断3
          string tem=getFirst(gra[i][3]); //求First[Y]
          for(int i=0;i<tem.length();i++){
            if(tem[i]!='@') t+=tem[i]; //去掉Y中的空
          }
          bool pan=false; //判断First[Y]中是否有空
          for(int i=0;i<tem.length();i++){
            if(tem[i]=='@'){  //找到空，pan为true
              pan=true;
              break;
            }
            else continue;
          }
          if(pan){  //只有当Y的First集中有空的时候才考虑以下规则
            if(gra[i].length()==4){ //若X–>Y，则将 ε 加入First(X)中
              t+='@';
              continue;
            }
            else if(gra[i][4]>='A'&&gra[i][4]<='Z'){  //若X–>YZ···，则将First(Z)加入First(X)中
              string temp="";
              temp=getFirst(gra[i][4]);
              for(int k=0;k<temp.length();k++) t+=temp[k];
            }
            else{ //若X–>Ya···，则将 a 加入First(X)中
              t+=gra[i][4];
            }
          }
          else continue;  //否则看下一个表达式
        }
        else{ //X->a……,后跟终结符
          t+=gra[i][3];  //若有产生式X->a……，把a放入t中
          continue;
        }
      }
      else continue;  //否则跳过
    }
    t=simpli(t);  //去除t中重复的字符
    t=SortS(t); //并按顺序排好
    return t;
  }
}
string getSFirst(string s){ //取得字符串s的first集，如aABC，ABa
  string t="";
  if(s.length()==0){  //空串直接返回空
    t+='@';
  }
  else{ //否则分两种情况
    if(s[0]>='A'&&s[0]<='Z'){ //字符串首个字符是非终结符
      string tem1=getFirst(s[0]);  //s[0]的first集
      string tem2=getSFirst(rearS(s)); //s去头的first集
      bool pan=false; //判断s[0]的first集中有没有空
      for(int i=0;i<tem1.length();i++){
        if(tem1[i]!='@') t+=tem1[i];
        else pan=true;
      }
      if(pan){  //假如pan为true，说明s[0]的first集中有@
        for(int i=0;i<tem2.length();i++) t+=tem2[i];  //把去头的s的first集放进去
      }
    }
    else{ //否则说明字符串首个字符是终结符
      t+=s[0];  //直接返回该终结符
    }
  }
  t=simpli(t);
  t=SortS(t);
  return t;
}
void showFirst(){ //展示所有非终结符和终结符的First集
  for(int i=0;i<gra.size();i++){  //遍历每一个文法产生式
    char t=gra[i][0];
    bool pan=false; //判断t是否已经在Vn中，之所以不看Vt是因为箭头左边必是非终结符
    for(int j=0;j<Vn.size();j++){
      if(Vn[j]==t){
        pan=true;
        break;
      }
      else continue;
    }
    if(pan==false) Vn.insert(Vn.begin()+Vn.size(),t); //t不在Vn中则放进去
    for(int j=3;j<gra[i].length();j++){ //从3开始是因为0已考虑过，1和2是箭头
      t=gra[i][j];
      if(t>='A'&&t<='Z'){ //非终结符
        pan=false; //判断t是否已经在Vn中，之所以不看Vt是因为箭头左边必是非终结符
        for(int k=0;k<Vn.size();k++){
          if(Vn[k]==t){
            pan=true;
            break;
          }
          else continue;
        }
        if(pan==false) Vn.insert(Vn.begin()+Vn.size(),t); //t不在Vn中则放进去
      }
      else{ //终结符
        pan=false; //判断t是否已经在Vn中，之所以不看Vt是因为箭头左边必是非终结符
        for(int k=0;k<Vt.size();k++){
          if(Vt[k]==t){
            pan=true;
            break;
          }
          else continue;
        }
        if(pan==false) Vt.insert(Vt.begin()+Vt.size(),t); //t不在Vn中则放进去
      }
    }
  }
  cout<<"下面展示所有非终结符的First集"<<endl;
  for(int i=0;i<Vn.size();i++){
    string t=getFirst(Vn[i]);
    cout<<Vn[i]<<"        ";
    for(int j=0;j<t.length();j++) cout<<t[j]<<"  ";
    cout<<endl;
  }
}
vector<vector<string>> I; //用来存放项目集族
vector<string> ii;  //用来向I中存放项目集,工具容器
struct Edge{
  int zuo;  //起始项目集的编号
  char zhong; //边上的非终结符或终结符
  int yo; //箭头指向项目集的编号
} bian[MAXN]={0}; //用来存放项目集之间的边的集合
int ne=0; //代表目前已存的边的个数
int has_existedi(vector<string> iii){ //判断iii是否已经在I中
  if(I.size()==0) return -1;  //假如I为空，直接返回-1
  int t=-1; //默认值为-1
  for(int i=0;i<I.size();i++){  //遍历项目集族I中的每一个项目集
    int ni=iii.size(),nI=I[i].size();  //ni为iii的表达式个数，nI为I[i]的表达式个数
    if(ni!=nI) continue;  //假如二者表达式个数不同，直接判断下一个项目集
    bool pan=true; //pan代表二者是否完全相等
    for(int j=0;j<ni;j++){  //遍历每一个表达式
      if(iii[j]!=I[i][j]){ //假如二者有某一个表达式不等
        pan=false;  //pan改成false
        break;  //退出循环
      }
    }
    if(pan){  //假如二者完全相等
      t=i;  //返回i
      break;
    }
    else continue;
  }
  return t; //假如一直没找到与ii完全相同的项目集，t还是默认值-1
}
bool xian(string a,string b){ //判断两个字符串的先后顺序，用来给项目集排序,true说明a排前边
  int na=a.length(),nb=b.length();
  if(na<nb) return true;  //a短，返回true
  else if(na>nb) return false;  //a长，返回false
  else{ //否则二者一样长
    for(int i=0;i<na;i++){
      if(a[i]<b[i]) return true;  //某一位a的字符比b的小，返回true
      else if(a[i]>b[i]) return false;  //某一位b的字符比a小，返回false
      else continue;  //否则说明二者相等，遍历向下一个字符
    }
    return false; //二者完全一致，返回false
  }
}
vector<string> Sorti(vector<string> iiii){  //将iiii中的表达式按照从小到大排序
  vector<string> iii;
  for(int i=0;i<iiii.size();i++) iii.insert(iii.begin()+iii.size(),iiii[i]);
  for(int i=0;i<(iii.size()-1);i++){ //很简单的冒泡排序
    for(int j=i+1;j<iii.size();j++){ //由于字符串与字符串本来就可以用<对比，所以不需太复杂的操作
      if(xian(iii[i],iii[j])) continue; //排在前的表达式字母顺序小则不用动
      else{ //否则简单的交换
        string tem=iii[j];
        iii[j]=iii[i];
        iii[i]=tem;
      }
    }
  }
  return iii;
}
vector<string> simplii(vector<string> iiii){  //去掉项目集iiii中多余的表达式
  vector<string> iii;
  for(int i=0;i<iiii.size();i++) iii.insert(iii.begin()+iii.size(),iiii[i]);
  iii=Sorti(iii); //首先排序iii
  vector<string> ti;  //工具容器的工具容器，用来存不重复的ii中表达式
  for(int i=0;i<iii.size();i++){ //遍历ii中的每个表达式，看是否已在ti中
    bool pan=false; //pan等于true说明ti中已有和ii[i]相等的表达式
    for(int j=0;j<ti.size();j++){ //遍历ti中已存在的每一个表达式
      if(ti[j]==iii[i]){ //假如ti中有和ii[i]相等的表达式，pan改成true，退出循环
        pan=true;
        break;
      }
      else continue;  //否则继续看ti中剩余的
    }
    if(pan) continue; //假如pan等于true那就不往ti里放了
    else{ //否则放进去
      ti.insert(ti.begin()+ti.size(),iii[i]);
    }
  } //for循环结束说明已经把该放的全部放完
  iii.clear(); //iii清空，重新往里填
  for(int i=0;i<ti.size();i++) iii.insert(iii.begin()+iii.size(),ti[i]); //依次填进去
  iii=Sorti(iii); //以防万一重新排下序
  return iii;
}
char afterNode(string s){ //求出一个表达式点后边的字符，工具函数
  for(int i=0;i<s.length();i++){
    if(s[i]=='`') return s[i+1];
    else continue;
  }
  return 'a'; //防止报错给一个返回值，其实用不到，反正我不会把这个函数用在没点的表达式上
}
vector<char> getdianh(vector<string> nows){ //取得当前项目集中点后的所有符号
  vector<char> t; //返回值
  for(int i=0;i<nows.size();i++){
    char tem=afterNode(nows[i]);    //得到nows中每一个表达式点后边的字符
    if(tem=='$') continue;//假如点后边就是逗号，不插入直接跳过
    t.insert(t.begin()+t.size(),tem); //插入
  }
  return t;
}
vector<string> getClosure(string nows){ //求每个闭包的项目集,并顺势求出与该项目集连接的边
  ii.clear(); //首先将ii清空，好让它发挥工具容器的职责（笑）
  ii.insert(ii.begin()+ii.size(),nows); //首先将nows放进去,ii.size其实是0，写习惯不想改了
  for(int i=0;i<ii.size();i++){ //遍历已经存进去的每一个表达式,由于界符中有逗号,所以将表达式中的逗号用$表示
    int dian=0,dou=0; //点代表表达式中·(`)的位置，dou代表表达式中逗号（$）的位置
    for(int j=0;j<ii[i].length();j++){  //先取得dian和dou的位置
      if(ii[i][j]=='`') dian=j;
      if(ii[i][j]=='$') dou=j;
    }
    if(ii[i][dian+1]=='$') continue;  //假如点后边就是逗号，例如S->S·,#  ，则不生成新的产生式，直接看下一条产生式
    if((ii[i][dian+1]<'A')||(ii[i][dian+1]>'Z')) continue;  //假如点后边是终结符，例如B->·aB,a/b  ，也不生成新的产生式，直接看下一条产生式
    else{ //否则说明点后边是非终结符，要根据文法表达式生成新的式子
      string newr=""; //newr代表新生成的表达式逗号($)后边的内容
      if(ii[i][dian+2]=='$'){ //假如点后两位直接就是逗号，如S->·S,# ，此时newr就是#
        for(int j=dou+1;j<ii[i].length();j++) newr+=ii[i][j];
      }
      else{ //否则说明点后两位不是逗号，如S->·BB,#，此时newr就是first(B#)
        string tem="";
        for(int j=dian+2;j<dou;j++) tem+=ii[i][j];
        tem=getSFirst(tem);
        bool pan=false; //pan为true代表first(tem)里有空
        for(int j=0;j<tem.length();j++){
          if(tem[j]!='@') newr+=tem[j]; //把不为空的放进去
          else pan=true;  //假如有空，空先不放进去，pan改成true
        }
        if(pan){  //pan为true说明有空
          for(int j=dou+1;j<ii[i].length();j++) newr+=ii[i][j]; //把逗号后边的放进去
        }
      }
      char nc=ii[i][dian+1];  //用nc存放点后边的第一个字符
      for(int j=0;j<gra.size();j++){  //遍历二型文法中的每一个表达式，看有没有箭头左侧是nc的
        if(gra[j][0]!=nc) continue; //假如gra[j]的第一个字符不等于nc则跳过，看下一条表达式
        else{ //否则有箭头左侧是nc的，需要加入项目集
          string temt="";
          for(int k=0;k<gra[j].length();k++){
            if(k==3) temt+='`'; //在箭头后边加入一个点
            temt+=gra[j][k];
          }
          temt+='$';  //末尾加上逗号
          for(int k=0;k<newr.length();k++) temt+=newr[k]; //把newr加入末尾
          ii.insert(ii.begin()+ii.size(),temt); //把新的表达式放入项目集
        }
      }
    }
  } //该for循环结束说明已经没有可以放进去的表达式了，此时ii放满了
  return ii;
}
bool has_existed_edge(int l,char m,int r){  //判断某一条边是否已在边集合中
  for(int i=0;i<ne;i++){  //遍历所有已存在的边
    if(bian[i].zuo==l&&bian[i].zhong==m&&bian[i].yo==r) return true;  //假如有一致的，返回true
    else continue;
  }
  return false; //都没有一致的则返回false
}
void putI(vector<string> nowi){ //把项目集nowi放入项目集族I中，并延伸出相对应的边
  int ni=has_existedi(nowi);  //nowi是否已在I中，不在为-1，在为对应序号
  if(ni==-1){ //nowi假如并不在I中
    ni=I.size();  //把ni从-1改成对应序号
    I.insert(I.begin()+I.size(),nowi);  //把nowi对应的项目集放进去
  }
  vector<char> nodec=getdianh(nowi);  //取得每一个表达式点后边的字符
  for(int i=0;i<nodec.size();i++){  //每个字符生成一个新的闭包项目集，随后生成一条边
    vector<string> iii; //用于暂时储存生成的项目集，因为一个符号可能对应多个表达式，要把这些表达式生成的项目集整合到一起并排序、去除多余表达式
    for(int j=0;j<nowi.size();j++){ //遍历每一个表达式
      char h=afterNode(nowi[j]);  //取得当前表达式点后边的字符
      if(h!=nodec[i]) continue; //假如当前表达式后边的字符不等于当前看的字符，则跳过，判断下一个表达式
      string ts=nowi[j];  //用ts保存，nowi[j]的点和点后第一个字符交换后的字符串(能到这一步说明点后边就是nodec[i])
      for(int l=0;l<ts.length();l++){
        if(ts[l]=='`'){ //假如ts中当前字符等于点
          char toolc=ts[l]; //交换
          ts[l]=ts[l+1];
          ts[l+1]=toolc;
          break;  //交换完了别忘了结束循环
        }
      } //结束循环时ts已经交换完毕
      vector<string> zani=getClosure(ts); //生成闭包项目集
      for(int l=0;l<zani.size();l++) iii.insert(iii.begin()+iii.size(),zani[l]);  //把zani中的每个表达式放入iii中
    }
    iii=Sorti(iii); //iii排序并去除多余表达式
    iii=simplii(iii);
    int niii=has_existedi(iii); //niii储存iii在I中位置，不在I中则为-1
    if(niii==-1){ //假如iii不在I中
      niii=I.size();  //令niii为当前I的项目个数
      I.insert(I.begin()+I.size(),iii); //随后把iii放进去
    }
    if(!has_existed_edge(ni,nodec[i],niii)){  //假如现在边集合中没有这条边
      bian[ne].zuo=ni;  //增加一条边，左边为ni
      bian[ne].zhong=nodec[i];  //中间为当前字符
      bian[ne].yo=niii; //右边为niii
      ne++; //别忘了把边数加一
    }
  }
}
string AG(int z,char c){  //用来求ACTION和GOTO的函数，两个输入分别为状态序号和动作
  if(c>='A'&&c<='Z'){ //假如c是非终结符，走GOTO
    for(int i=0;i<ne;i++){
      if(bian[i].zuo==z&&bian[i].zhong==c){ //找到对应的边
        int t=bian[i].yo;
        string tt="";
        tt+=to_string(t);
        return tt;
      }
    }
  }
  else{ //否则c是终结符，走ACTION
    for(int i=0;i<ne;i++){  //首先判断有没有从Iz出发，中间是c的边
      if(bian[i].zuo==z&&bian[i].zhong==c){ //找到了
        string S="S";
        int Sy=bian[i].yo;
        S+=to_string(Sy);
        return S; //返回Si
      }
      else continue;    //否则继续看下一条边
    } //for循环结束还没return说明不是移进
    for(int i=0;i<I[z].size();i++){ //遍历Iz中每一个表达式
      int dian=0; //记录点的位置
      for(int j=0;j<I[z][i].length();j++){
        if(I[z][i][j]=='`'){
          dian=j; //记录点的位置
          break;
        }
        else continue;  //否则继续找下一个
      } //for循环结束的时候dian已经记录好位置了
      if(I[z][i][dian+1]=='$'){ //只有点后面就是逗号的才考虑
        string rS;
        if(I[z][i][dian-1]=='S'){ //假如点前面就是S，返回acc
          rS="acc";
        }
        else{ //否则返回rl
          rS="r";
          string toolS="";
          for(int l=0;l<dian;l++){  //用toolS存放当前表达式点前面的部分
            toolS+=I[z][i][l];
          }
          for(int l=0;l<gra.size();l++){
            if(toolS==gra[l]){
              rS+=to_string(l);
              break;
            }
          }
        }
        for(int l=dian+2;l<I[z][i].length();l++){ //dian+2是逗号后第一个符号，看有没有和c相同的
          if(I[z][i][l]==c){
            return rS;
          }
          else continue;
        }
      }
      else continue;  //否则看下一个表达式
    }
  }
  string wr="   ";  //代表wrong
  return wr;
}
int wei(int A){ //求a的位数，工具函数
  if(A==0) return 1;
  int a=A,w=0;
  while(a>0){
    w++;
    a/=10;
  }
  return w;
}
void showFXB(){ //展示分析表
  cout<<"如下为LR(1)分析表,其中终结符下位ACTION表,非终结符下位GOTO表";
  cout<<endl<<"____"; //内容前的横线
  for(int i=0;i<Vt.size();i++) cout<<"______";
  for(int i=0;i<Vn.size();i++) cout<<"_____";
  cout<<endl<<"I  |"; //表头
  for(int i=0;i<Vt.size();i++) cout<<"  "<<Vt[i]<<"  |";  //终结符和非终结符
  cout<<"  "<<'#'<<"  |";
  for(int i=1;i<Vn.size();i++){
    cout<<"  "<<Vn[i]<<" |";
  }
  cout<<endl<<"______"; //内容前的横线
  for(int i=0;i<Vt.size();i++) cout<<"______";
  for(int i=0;i<Vn.size();i++) cout<<"_____";
  cout<<endl;
  for(int i=0;i<I.size();i++){  //每个状态打印一行
    cout<<i;
    if(wei(i)==1) cout<<"  "; //由于最长是三位，所以假如状态是1位就补两位
    else if(wei(i)==2) cout<<" "; //否则补1位
    cout<<"|";
    for(int j=0;j<Vt.size();j++){
      cout<<" "<<AG(i,Vt[j]);
      if(AG(i,Vt[j]).length()==3) cout<<" ";
      else if(AG(i,Vt[j]).length()==2) cout<<"  ";
      else if(AG(i,Vt[j]).length()==4) cout<<"";
      else if(AG(i,Vt[j])=="   ") cout<<" ";  //将ACTION表的数字统一输出位5位
      cout<<"|"; //用来分隔
    }
    cout<<" "<<AG(i,'#'); //由于Vt里没有#，所以需要单独输出一列
    if(AG(i,'#').length()==3) cout<<" ";
    else if(AG(i,'#').length()==2) cout<<"  ";
    else if(AG(i,'#').length()==4) cout<<"";
    else if(AG(i,'#')=="   ") cout<<" ";  //将ACTION表的数字统一输出位5位
    cout<<"|"; //用来分隔
    for(int j=1;j<Vn.size();j++){ //由于Vn的第一个元素是T，是开始符号，所以j从1开始而不是0
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
  for(int i=0;i<Vn.size();i++) cout<<"_____";  //表尾
  cout<<endl; //转到下一行，便于一会儿分析输入的token
}
void AnalyS(){  //分析函数
  string tokenS="";
  for(int i=0;i<tok.size();i++) tokenS+=tok[i]; //将tok容器的内容取出来
  tokenS+="#";  //输入串末尾加上#
  int bu=0; //分析步骤，初始为0
  cout<<"步骤"<<" "<<"状态栈"<<' '<<" 符号栈   输入串";
  for(int i=0;i<tokenS.length();i++) cout<<" ";
  cout<<"        操作";
  vector<int> Sta; //状态栈
  Sta.insert(Sta.begin(),0);  //状态栈初始化为{0}
  vector<char> Symb; //符号栈
  Symb.insert(Symb.begin(),'#'); //符号栈初始化为{#}
  cout<<endl;
  while(bu<100000){ //其实可以写while(1),为防止文法出错导致死循环，设置一个跳出条件
    bu++; //步骤数加一
    cout<<bu<<"    ";
    if(wei(bu)==1) cout<<"  "; //为了整齐
    else if(wei(bu)==2) cout<<" ";
    for(int l=0;l<Sta.size();l++) cout<<Sta[l]<<' ';
    cout<<"    ";
    for(int l=0;l<Symb.size();l++) cout<<Symb[l]<<' ';
    cout<<"    "<<tokenS<<"                ";
    int zhuang=Sta[Sta.size()-1]; //取状态栈栈顶，存储在zhuang中
    char shu=tokenS[0]; //取输入串头，存储在shu中
    string dang=AG(zhuang,shu); //对应表中的值
    if(dang=="acc"){  //若为acc则分析成功
      cout<<"成了，牛逼"<<endl<<"SUCCESS!!!!!";
      return;
    }
    else if(dang=="   "){ //分析失败，即该文法有语法错误，或遇到@
      string Aj=AG(zhuang,'@');
      if(Aj=="   "){
        cout<<"寄了"<<endl<<"FAILURE";  //假如不是遇到@，分析出错
        string tokenSS="";
        for(int h=0;h<tok.size();h++) tokenSS+=tok[h]; //将tok容器的内容取出来
        int chu=0;  //记录输入串中出错符号出现次数
        for(int h=0;h<tokenSS.length();h++){
          if(Symb[Symb.size()-1]==tokenSS[h]) chu++;
        }
        for(int h=0;h<tokenSS.length();h++){
          if(Symb[Symb.size()-1]==tokenSS[h]){
            if(chu==1){
              cout<<endl<<"出错行号在"<<hangh[h]<<"行，请检查";
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
        int sj=atoi(Sj.c_str());  //string转int
        Sta.insert(Sta.begin()+Sta.size(),sj);
        Symb.insert(Symb.begin()+Symb.size(),'@');
        cout<<"需要用空符号来规约，因此在状态栈压入"<<sj<<"，在符号栈中压入@";
      }
    }
    else if(dang[0]=='S'){  //移进
      string Sj="";
      for(int l=1;l<dang.length();l++) Sj+=dang[l]; //取得S后边的部分，用来转成int
      int sj=atoi(Sj.c_str());  //string转int
      Sta.insert(Sta.begin()+Sta.size(),sj); //sj入状态栈
      Symb.insert(Symb.begin()+Symb.size(),shu); //串头入符号栈
      tokenS=rearS(tokenS); //输入串去掉串头
      cout<<"ACTION("<<zhuang<<','<<shu<<")="<<dang<<"，移进操作";
    }
    else{ //规约,首字符是r
      string rj="";
      for(int l=1;l<dang.length();l++) rj+=dang[l]; //取得r后边的部分，用来转成int
      int nr=atoi(rj.c_str());  //string转int
      char lef=gra[nr][0];  //rj表达式左部
      string rig="";  //rj表达式右部
      for(int l=3;l<gra[nr].length();l++) rig+=gra[nr][l];  //取箭头右边的部分
      int nrig=rig.length();
      for(int l=0;l<nrig;l++) Symb.pop_back();
      Symb.insert(Symb.begin()+Symb.size(),lef); //进行规约
      for(int l=0;l<nrig;l++) Sta.pop_back();  //状态栈出栈
      int kkk=Sta[Sta.size()-1];  //取状态栈栈顶
      string GOTOkX=AG(kkk,lef);  //求GOTO
      int gotokx=atoi(GOTOkX.c_str());  //转int
      Sta.insert(Sta.begin()+Sta.size(),gotokx); //GOTO入栈
      cout<<"ACTION("<<zhuang<<','<<shu<<")="<<dang<<"，规约操作";
    }
    cout<<endl;
  }
}
int main(){
  load_Grammar(); //加载2型文法
  load_Token(); //加载token
  showFirst();  //展示每个非终结符的First集
  string t="T->`S$#"; //T为新添加的开始非终结符，相当于S'，`代表点，$代表逗号
  vector<string> ti=getClosure(t);  //首先求t的闭包项目集
  putI(ti); //先将ti放入I中，并生成与它关联的新的项目集、边
  for(int i=0;i<I.size();i++) putI(I[i]); //随后将I中的每个项目集进行相同操作
  cout<<"LR(1)项目集之间的关系如下"<<endl;
  for(int i=0;i<ne;i++){
    cout<<bian[i].zuo<<"        "<<bian[i].zhong<<"        "<<bian[i].yo<<endl;
  }
  cout<<"每个项目集代表的表达式如下："<<endl;
  for(int i=0;i<I.size();i++){
    cout<<"I"<<i<<":"<<endl;
    for(int j=0;j<I[i].size();j++){
      for(int l=0;l<I[i][j].length();l++){
        if(I[i][j][l]=='`') cout<<"·";  //将`变回点
        else if(I[i][j][l]=='$') cout<<","; //将$变回逗号
        else cout<<I[i][j][l];
      }
      cout<<endl;
    }
    cout<<endl;
  }
  showFXB();  //展示LR(1)分析表
  AnalyS(); //分析输入串
  getchar();
  return 0;
}