//A C++ PROGRAM TO PRINT PLA(PROGRAMMABLE LOGIC ARRAY) PROGRAM TABLE.


#include<iostream>
#include<cmath>

using namespace std;


struct f_number_function
{
	int term_original[100];                                      //function for storing the number of variables in F and F'.
	int term[100];
	int v;
};



struct Minterm
{
	int num;                                                    //a structure defining a minterm.
	int bin[100]={0};
	int countOne=0;
	bool check=false;
};





int var,maxV,f_number,f_no,mt,d,plaSMin=999999,plaCode,sz;       //global variables.


f_number_function f_numbernc[500][500],pla[500];
Minterm minT[500];
Minterm table1[500];
Minterm table2[500];





int reset1()
{
	for(int i=0;i<500;i++)
	{
		for(int j=0;j<100;j++)
			minT[i].bin[j]=0;                                     //function for resetting the values of Minterm.
		minT[i].countOne=0;
		minT[i].num=0;
		minT[i].check=false;
	}
}





int reset2()
{
	for(int j=0;j<500;j++)
	{
		for(int k=0;k<100;k++)                                        //function for resetting structure f_numbernc.
		{
			f_numbernc[f_no][j].term[k]=9;
		}
	}
}





int reset3()
{
	for(int j=0;j<500;j++)
	{
		for(int k=0;k<100;k++)
		{
			pla[j].term[k]=9;                                         //function for resetting PLA.
		}
	}
}




int begin()
{
	for(int i=0;i<500;i++)                            //for setting f_numbernc initially at high values.
	{
		for(int j=0;j<100;j++)
			pla[i].term[j]=9;
		for(int j=0;j<500;j++)
			for(int k=0;k<100;k++)
			{
				f_numbernc[i][j].term[k]=9;
				f_numbernc[i][j].term_original[k]=9;
			}
	}
}








int sort()
{                                                     //function for arranging terms in order of number of 1's.
	int min;
	for(int i=0;i<mt-1;i++)
	{
		min=i;
		for(int j=0;j<mt-1-i;j++)
			if(minT[j].countOne>minT[j+1].countOne)
			{     Minterm temp;
	              temp=minT[i];
                  minT[i]=minT[j];
	              minT[j]=temp;
			}
	}
}





int pair2(Minterm temp[500],int size);
int pair1(Minterm temp[500],int size)                              //functions for optimization of F by recursion.
{
	for(int i=0;i<size;i++)
		temp[i].check=false;
	int count,pos,l=0;
	for(int i=0;i<size;i++)
	{
		for(int j=i+1;j<size;j++)
		{
			if(temp[j].countOne==temp[i].countOne+1)
			{
				count=0;
				for(int k=0;k<var;k++)
				{
					if(temp[i].bin[k]!=temp[j].bin[k])
					{
						count++;
						pos=k;
					}
				}
				if(count==1)                                                   //pairing on the basis of the number of 1's.
				{
					table1[l]=temp[i];
					table1[l].bin[pos]=9;
					l++;
					temp[i].check=true;
					temp[j].check=true;
				}
			}
		}
	}
	for(int i=0;i<size;i++)
		if(temp[i].check==false)
		{
			for(int j=0;j<var;j++)
				f_numbernc[f_no][sz].term[j]=temp[i].bin[j];
			sz++;
		}
	if(l!=0)
		pair2(table1,l);
}




int pair2(Minterm temp[500],int size)
{                                                                     //function for pairing in next group.
	for(int i=0;i<size;i++)
		temp[i].check=false;
	int count,pos,l=0;
	for(int i=0;i<size;i++)
	{
		for(int j=i+1;j<size;j++)
		{
			if(temp[j].countOne==temp[i].countOne+1)
			{
				count=0;
				for(int k=0;k<var;k++)
				{
					if(temp[i].bin[k]!=temp[j].bin[k])
					{
						count++;
						pos=k;
					}
				}
				if(count==1)                                                 //further pairing in next group.
				{
					table2[l]=temp[i];
					table2[l].bin[pos]=9;
					l++;
					temp[i].check=true;
					temp[j].check=true;
				}
			}
		}
	for(int i=0;i<size;i++)
		if(temp[i].check==false)
		{
			for(int j=0;j<var;j++)
				f_numbernc[f_no][sz].term[j]=temp[i].bin[j];
			sz++;
		}
	}
	cout<<endl;
	if(l!=0)
		pair1(table2,l);                                                         //recursion for further optimization of F.
}





int f_numberncDetail()
{
	int co=0;                                              //function to exclude repeating terms.
	for(int i=0;i<mt;i++)
	{
		for(int n=minT[i].num,j=var-1;n>0;n/=2,j--)
		{
			minT[i].bin[j]=n%2;
			if(n%2==1)
				minT[i].countOne++;
		}
	}
	sort();
	pair1(minT,mt);
	for(int i=0;i<sz;i++)
	{
		for(int j=i+1;j<sz;j++)
		{
			co=0;
			for(int k=0;k<var;k++)
			{
				if(f_numbernc[f_no][i].term[k]==f_numbernc[f_no][j].term[k])
					co++;
			}
			if(co==var)
				f_numbernc[f_no][j--]=f_numbernc[f_no][--sz];
		}
	}
}





int plaMinimize(int t)
{
	int co;
	for(int i=0;i<t;i++)
	{
		for(int j=i+1;j<t;j++)
		{
			co=0;
			for(int k=0;k<100;k++)                                       //function for collecting and removing common terms.
			{
				if(pla[i].term[k]==pla[j].term[k])
					co++;
			}
			if(co==100)
				pla[j--]=pla[--t];
		}
	}
	return t;
}





int plaSelection(int fn,int q)
{
	for(int i=0;i<((q==0)?(f_numbernc[fn][1].v):(f_numbernc[fn][2].v));i++)
	{
		for(int j=0;j<100;j++)
		{                                                                   //function for deciding best combination among F'
			if(q==0)
				pla[d].term[j]=f_numbernc[fn][i].term_original[j];
			else
				pla[d].term[j]=f_numbernc[fn][i].term[j];
		}
		d++;
	}
}





int plaSelection_original()
{
	for(int i=0;i<pow(2,f_number);i++)
	{
		d=0;
		reset3();                                                        //Function for deciding best combination among F
		for(int k=0;k<f_number;k++)
		{
			int a=pow(2,k);
			int b=(i/a)%2;
			plaSelection(k,b);
		}
		d=plaMinimize(d);
		if(d<plaSMin)
		{
			plaSMin=d;
			plaCode=i;
		}
	}
}





int plaFinal()
{
	d=0;                                             //function for deciding best pair among F and F'
	reset3();
	for(int k=0;k<f_number;k++)
	{
		int a=pow(2,k);
		int b=(plaCode/a)%2;
		for(int i=0;i<((b==0)?(f_numbernc[k][1].v):(f_numbernc[k][2].v));i++)
		{
			for(int j=0;j<100;j++)
			{
				if(b==0)
					pla[d].term[j]=f_numbernc[k][i].term_original[j];
				else
					pla[d].term[j]=f_numbernc[k][i].term[j];
			}
			pla[d].v=f_numbernc[k][0].v;
			d++;
		}
	}
}





int PLA_TABLE_DISPLAY()                                                    //function for displaying PLA table.
{
	cout<<endl<<endl<<"\n\n\n\n\t\t\t\t***** PLA PROGRAM TABLE ****\n\n"<<endl<<endl;
	cout<<"\t\t| Product terms |\tInput\t\t\t|\tOutput"<<endl;
	cout<<"\t\t|\t\t|\t";
	for(int i=0;i<maxV;i++)
		cout<<(char)(97+i);
	cout<<"\t\t\t|";
	for(int i=0;i<f_number;i++)
		cout<<"\tF"<<i+1;
	cout<<endl;
	cout<<"-----------------------------------------------------------------------------------------------------------------"<<endl;
	for(int i=0;i<d;i++)
	{
		for(int j=0;j<pla[i].v;j++)
		{
			if(pla[i].term[j]==1)
				cout<<(char)(65+j);
			if(pla[i].term[j]==0)
				cout<<(char)(65+j)<<"'";
		}
		cout<<"\t\t|\t"<<i+1<<"\t|\t";
		for(int j=0;j<pla[i].v;j++)
		{
			if(pla[i].term[j]==9)
				cout<<"_";
			else
				cout<<pla[i].term[j];
		}
		cout<<"\t\t\t|\t";
		for(int j=0;j<f_number;j++)
		{
			bool cond1=false;
			int a=pow(2,j);
			int b=(plaCode/a)%2;
			for(int k=0;k<500;k++)
			{
				bool cond2=true;
				for(int h=0;h<100;h++)
				{

					if(b==0)
					{
						if(pla[i].term[h]!=f_numbernc[j][k].term_original[h])
							cond2=false;
					}
					else
					{
						if(pla[i].term[h]!=f_numbernc[j][k].term[h])
							cond2=false;
					}
				}
				if(cond2)
				{
					cout<<"1\t";
					cond1=true;
					break;
				}
			}
			if(!cond1)
			{
				cout<<"-\t";
			}
		}
		cout<<endl;
	}
	cout<<"-----------------------------------------------------------------------------------------------------------------"<<endl;
}






//MAIN FUNCTION.





int main()
{
	begin();
	int co;
	cout<<"\t\t\tPROGRAM TO PRINT PLA(PROGRAMMABLE LOGIC ARRAY) PROGRAM TABLE.\n\n";
	cout<<"\n\nEnter the number of function you want to realize with PLA : ";
	cin>>f_number;

	for(f_no=0;f_no<f_number;f_no++)
	{
		sz=0;
		reset1();
		cout<<"\n\n\nFor the function "<<f_no+1;
		cout<<" enter the number of variables : ";                    //entering basic inputs for functions and minterms in SOP form.
		cin>>var;
		f_numbernc[f_no][0].v=var;
		if(var>maxV)
			maxV=var;
		cout<<"Enter the number of minterms in function: ";
		cin>>mt;
		cout<<"Enter the minterms one by one :\n";
		for(int i=0;i<mt;i++)
			cin>>minT[i].num;
        bool chh=false;
        cout<<"\n\n";
        for(int i=0;i<mt;i++)
        {
            if((minT[i].num<0)|| (minT[i].num>=pow(2,var)))              //conditions for checking of out of bound minterms.
            {
                cout<<minT[i].num<<"  ";
                minT[i--]=minT[--mt];
                chh=true;
            }
        }
        for(int i=0;i<mt;i++)
        {
            for(int j=i+1;j<mt;j++)
            {
                if(minT[i].num==minT[j].num)         //checking for repeating minterms.
                minT[j--]=minT[--mt];
            }
        }
        for(int i=0;i<mt-1;i++)
        {
            for(int j=0;j<mt-1-i;j++)
            {
                if(minT[j].num>minT[j+1].num)
                {   Minterm t;
                    t=minT[j];
                    minT[j]=minT[j+1];
                    minT[j+1]=t;
                }
            }

        }

        if(chh)
         cout<<"found to be invalid minterms and will be removed from further calculations.\n";
        cout<<"\nMinterms of F"<<f_no+1<<" are  ";
        for(int i=0;i<mt;i++)
          cout<<minT[i].num<<", ";
        cout<<"\n";
		f_numberncDetail();
		for(int i=0;i<500;i++)
		{
			for(int k=0;k<100;k++)
			{
				f_numbernc[f_no][i].term_original[k]=f_numbernc[f_no][i].term[k];
			}
		}
		cout<<"\n";
		reset2();
		cout<<"F"<<f_no+1<<" = ";
		if(mt==pow(2,var))
        cout<<"1\n";
        else if(mt==0)
            cout<<"0\n";
        else
        {
	       for(int i=0;i<sz;i++)
		     {
			   for(int j=0;j<var;j++)                       //printing minimized function F in SOP form
                {
				  if(f_numbernc[f_no][i].term_original[j]==1)
					cout<<(char)(65+j);
				  if(f_numbernc[f_no][i].term_original[j]==0)
					cout<<(char)(65+j)<<"'";
                }
			cout<<" + ";
		    }
		cout<<"\b\b "<<"\n";
        }
		f_numbernc[f_no][1].v=sz;
		Minterm temp[500];
		bool ch;
		int a=0;
		for(int i=0;i<pow(2,var);i++)
		{
			ch=false;
			for(int j=0;j<mt;j++)
			{
				if(minT[j].num==i)
					ch=true;
			}
			if(!ch)
				temp[a++].num=i;
		}


		cout<<"\b\b "<<endl;
		sz=0;
		reset1();
		mt=a;
		for(int i=0;i<a;i++)
			minT[i].num=temp[i].num;
		f_numberncDetail();

		cout<<"F"<<f_no+1<<"' = ";
        if(mt==(int)pow(2,var))                                   //printing for minimized function F'.
		 cout<<"1\n";
        else if(mt==0)
         cout<<"0\n";
        else
        {
            for(int i=0;i<sz;i++)
            {
                for(int j=0;j<var;j++)
                {
                    if(f_numbernc[f_no][i].term[j]==1)
                        cout<<(char)(65+j);
                    if(f_numbernc[f_no][i].term[j]==0)
                        cout<<(char)(65+j)<<"'";
                }
                cout<<" + ";
            }
            cout<<"\b\b "<<endl;
        }
		f_numbernc[f_no][2].v=sz;
	}
	plaSelection_original();
	plaFinal();                                               //printing PLA table with some conventions given below.
	d=plaMinimize(d);
	PLA_TABLE_DISPLAY();
	cout<<"\t\t\t\t\t\t\t\t|\t";
	for(int k=0;k<f_number;k++)
	{
		int a=pow(2,k);
		int b=(plaCode/a)%2;
		if(b==0)
			cout<<"T\t";
		else
			cout<<"C\t";
	}
	cout<<"\n\n\t\t\t\t\t\t\t\t\t(T=True/C=compliment)\n";
	cout<<"\n\n\n";
}

