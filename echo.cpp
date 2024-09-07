#include<iostream>
//#include<bits/stdc++.h>
#include <string>


#include "echo.h"

using namespace std;


extern string homeDir;
bool echo(string cmd)
{


  // cmd [0] is pointing to first valid character
int k;
for( k=cmd.size()-1;k>=0 and cmd[k]==32;k--); // k is pointing to last valid character 
//  cout<<"k="<<k<<endl;

bool background=false;
if(cmd[k]=='&')
{
    background=true;
    

    cout<<"there is &"<<endl;

    // handling it letter
    return true;
   

}

// k is pointing to last character  which we have to print 

// int x=0;
bool flag=false;
if(cmd[0]==cmd[k] and (cmd[0]==34 or cmd[0]==39) )
{

    // cout<<"iam here";
  flag=true;
  
} 



    // string mid=cmd.substr(j,k-j+1);
    
    int x=0;

    int l=cmd.size();
    if(flag) {l--;x++;}

    // cout<<"l="<<l<<" x="<<x<<endl;
    while(x<l)
    {
        while(x<l and cmd[x]!=32)
        {
            
           cout<<cmd[x];
           x++;
        }
        
        if(flag==false and x<l)
        cout<<" ";

       while(x<l and cmd[x]==32)
        {
           if(flag) cout<<cmd[x];
           x++;
        } 


    }


    cout<<endl;
   return true;

}