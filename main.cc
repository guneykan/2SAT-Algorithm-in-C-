// Tarjan's Algorithm and Solution to 2SAT problem.
//
// Author  : <Güneykan Özgül> 
//           Bogazici University, Computer Engineering
// Project :Solution to 2SAT problem using graph theory.
//The code below is only for illustration purposes and neither well optimised nor is the best way of implementing any method.
#include <iostream>
#include <forward_list>
#include <fstream>
#include <queue>
#include <vector>
#include <stack>

// Compile command:  g++ --std=c++11 main.cc -o sat
// Run command 1:    ./sat filename
using namespace std;
using AdjList = std::vector<std::vector<int>>;
using EdgeList = std::vector<std::pair<int, int>>;

class Graph{
public:
	int N;
	int* index,*lowlink;
	bool* truth,*visited,*stackmember;
	std::stack<int> st;
	int in;
	// Input file format:
	//  - first line:N number of propositions(Not including negations).
	//  - remaining lines ("x || y" pairs).
	//Negation of  proposition x represented by x+N.
	Graph(std::string filename){
		std::ifstream fin(filename);
		if(!fin.is_open()){
			return;
		}
		int num_vertices,x, y;
		fin>>num_vertices;
		N=num_vertices;
		index=new int[2*num_vertices];
		truth=new bool[2*num_vertices];			
		visited=new bool[2*num_vertices];
		lowlink=new int[2*num_vertices];
		stackmember=new bool[2*num_vertices];
		adjlist.resize(2*num_vertices);
		while(fin>>x>>y){
			//Graph consists of implications. Add ~p->q and ~q->p instead to the graph.   			
			adjlist[x].push_back(y);
			adjlist[y].push_back(x);
		}
		fin.close();
	}
	//Print the implication graph.
	void Print(){
		int nodelabel = 0;
		std::cout<<"Adjacency List:\n";
		for(auto& nodes_list : adjlist){
			std::cout<< nodelabel++ <<" -> ";
			for(int node : nodes_list){
				std::cout<<node<<" ";
			}
			std::cout<<std::endl;
		}
	}	
	/*Detect strongly connected components by using Tarjan's algorithm. In the condensation of the implication graph assign every element in a component
	same truth value. Since Tarjan's algorithm gives the components in reverse topological order there is no need to topologically order components again.*/ 
	void SCC(){
		in=0;
		while (!st.empty()) st.pop();
		for(int i=0;i<adjlist.size();i++){
			index[i]=0;
			visited[i]=false;
			lowlink[i]=0;
			stackmember[i]=false;

		}		
		bool satisfiable=true;
		for(int i=0;i<adjlist.size();i++){
			if(index[i]==0){
				if(!strongconnect(i)){ satisfiable=false; break;}
			}
		}
		if(satisfiable){
			printtable();	
		}else{printf("System is unsatisfiable.");}
	}
	bool strongconnect(int v){
		visited[v]=true;
		index[v]=++in;
		lowlink[v]=in;
		st.push(v);
		stackmember[v]=true;
		for(int i=0;i<adjlist[v].size();i++){
			if(index[adjlist[v][i]]==0){
				strongconnect(adjlist[v][i]);
				lowlink[v]  = std::min(lowlink[v], lowlink[adjlist[v][i]]);
			}else if(stackmember[adjlist[v][i]]){
				lowlink[v]  = std::min(lowlink[v], lowlink[adjlist[v][i]]);
			}
		}	
		if (index[v] == lowlink[v]){
			int w=0;
			while(st.top()!= v) {
				int w = st.top();
				if(!assign(w)) { return false;}
				stackmember[w] = false;
				st.pop();
				
			}     
			w = st.top();
			if(!assign(w)){return false; }
			stackmember[w] = false;
			st.pop();

			
		}
		return true;
	}
	//Assign false to the proposition and assign true to complementary proposition.
	bool assign(int x){
		if(x<N){
			if(lowlink[x]!=lowlink[x+N]){
				truth[x]=false;
				truth[x+N]=true;
				return true;
			}else{return false;}
		}else{
			if(lowlink[x]!=lowlink[x-N]){
				truth[x]=false;
				truth[x-N]=true;
				return true;
			}else{return false;}
		}
	}
	//Print the given system and solution found.
	void printtable(){


		int nodelabel = 0;
		std::cout<<"Table\n";
		for(auto& nodes_list : adjlist){
			for(int node : nodes_list){
				if(node<N&&nodelabel<N&&node>=nodelabel)
				std::cout<<"(X"<<nodelabel<<" || X"<<node<<") && ";
				else if(node>=N&&nodelabel<N&&node>=nodelabel)
				std::cout<<"(X"<<nodelabel<<" || ~X"<<node-N<<") && ";	
				else if(node<N&&nodelabel>=N&&node>=nodelabel){}

				else if(node>=N&&nodelabel>=N&&node>=nodelabel){
					std::cout<<"(~X"<<nodelabel-N<<" || ~X"<<node-N<<") && "; 

				}					
			}
			
			nodelabel++;
		}
		std::cout<<"\nSolution: "<<std::endl;
		
		
		nodelabel = 0;
		
		for(auto& nodes_list : adjlist){
			for(int node : nodes_list){
				if(node<N&&nodelabel<N&&node>=nodelabel){
					std::cout<<"( "<<truth[nodelabel]<<" ||  "<<truth[node]<<") && ";}
				else if(node>=N&&nodelabel<N&&node>=nodelabel){
					std::cout<<"( "<<truth[nodelabel]<<"  ||  "<<truth[node]<<") && ";
				}
				else if(node>=N&&nodelabel>=N&&node>=nodelabel){
					std::cout<<"( "<<truth[nodelabel]<<"  ||   "<<truth[node]<<") && ";
				}
				

			}
			nodelabel++;

		}
	}
private:
	AdjList adjlist;
};

int main(int argc, char*argv[]) {
	Graph *G;
	if(argc < 2){
		printf("Call it with a file argument");
	} else {
		G = new Graph(argv[1]);
	}
	G->SCC();
	return 0;
}
