#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <cstdio>
#include <chrono>


class Square {
    
    public:

        Square(int sym) {
            symbol = sym;
            visited = false;
        }

    public:
        int symbol;
        int row;
        int col;
        bool visited;
        std::vector<Square*> adj_list;
};

int n;
int m;
std::vector<std::vector<Square*> > map;
std::vector<Square*> virus_list;
std::vector<Square*> space_list;

void print_map();
int _max_sum = 0;

int bfs() {
    int sum = 0;
    std::queue<Square*> que;
    std::vector<Square*> visited;

    Square* curr_virus;
    Square* curr;
    Square* neigh;

    for(int i=0; i < virus_list.size(); i++) {
        curr_virus = virus_list[i];
        que.push(curr_virus);
    }
    
    while(!que.empty()) {
        curr = que.front(); 
        que.pop();

        curr->visited = true;
        visited.push_back(curr);
        
        for(int j=0; j< curr->adj_list.size(); j++){
            Square* node;
            node = curr->adj_list[j];

            if(node->symbol == 0 && !node->visited)
                que.push(node);
        }
    }
    
    for(int i=0; i < space_list.size(); i++) {
       if(space_list[i]->symbol == 0 && !space_list[i]->visited) 
            sum++;
    }

    for(int i=0; i<visited.size(); i++) {
        visited[i]->visited = false;
    }
    
    return sum;
}


void dfs(int cnt) {
    
    int sum;
    if(cnt == 3) {
        
        sum = bfs();
        if(sum > _max_sum)
            _max_sum = sum;
 
        return;
    }

    else { 
        for(int i=0; i < space_list.size(); i++) {
            Square* curr = space_list[i];
            if(curr->symbol == 0) {
                curr->symbol = 1;
                dfs(cnt+1);    
                curr->symbol = 0;
            }
        }
    }
}
void print_map() {

    for(int i=0; i<n; i++) {
	    for(int j=0; j<m; j++) {
		    std::cout << map[i][j]->symbol << " ";
		}
		std::cout << '\n';;
	}
}

void update_adj() {
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            
            Square* curr = map[i][j];
            if(curr->symbol != 1) {
                Square* neigh;

                int curr_row = curr->row;
                int curr_col = curr->col;

                if (curr_row + 1 < n) {
                    neigh = map[curr_row+1][curr_col];
                    if(neigh->symbol == 0)
                        curr->adj_list.push_back(neigh);
                }   

                if(0 <= curr_row - 1) {
                    neigh = map[curr_row-1][curr_col];
                    if(neigh->symbol == 0)
                        curr->adj_list.push_back(neigh);

                }

                if( curr_col + 1 <  m) {
                    neigh = map[curr_row][curr_col+1];
                    if(neigh->symbol == 0)
                        curr->adj_list.push_back(neigh);
                }

                if(0 <=  curr_col -1) {
                    neigh = map[curr_row][curr_col-1];
                    if(neigh->symbol ==0)
                        curr->adj_list.push_back(neigh);
                }
            }
        }
    }
}
/*
std::vector<std::vector<Square*> > combination(std::vector<Square*> space_list,
                                   int n, int r, int index, 
                                   std::stack<Square*> stack, 
                                   std::vector<std::vector<Square*> > result) {
    
    if(r == 0) {

        std::stack<Square*> temp = stack;
        std::vector<Square*> record;
        
        int i =0;
        while(i < 3) {
            record.push_back(temp.top());
            temp.pop();
            i++;
        }

        if(!record.empty())
            result.push_back(record);
        
        return result;
    }
    
    else if( r > n) {
        
        if(!stack.empty()) 
            stack.pop();

        return result;
        
    }
    else {
        Square* curr = space_list[index];
        stack.push(curr);

        result = combination(space_list,n-1,r-1,index+1,stack,result);
        stack.pop();
        result = combination(space_list,n-1,r,index+1,stack,result);

        return result;
    }
}

std::vector<std::vector<Square*> > do_combi() {

    std::stack<Square*> stack;
    std::vector<std::vector<Square*> > result;
    int n = space_list.size();

    return combination(space_list,n,3,0,stack,result);
}
*/
int main() {	
    std::chrono::high_resolution_clock::time_point begin;
    std::chrono::high_resolution_clock::time_point end;

    begin = std::chrono::high_resolution_clock::now();
    // read size of the map
    std::cin >> n >> m;
    
	for(int i = 0; i< n; i++) {
		std::vector<Square*> record;
	    for(int j = 0; j < m; j++) {
		    int temp;
		    std::cin >> temp;
            Square* sq = new Square(temp);
            sq->row = i;
            sq->col = j;
            record.push_back(sq);

            if(temp == 0)
                space_list.push_back(sq);
            
            else if(temp == 2)
                virus_list.push_back(sq);

	    }
	  map.push_back(record);
	}
    
    std::chrono::high_resolution_clock::time_point d1;    
    std::chrono::high_resolution_clock::time_point d2;
    d1 = std::chrono::high_resolution_clock::now();
    update_adj();
    d2 = std::chrono::high_resolution_clock::now();

    long long d_time = std::chrono::duration_cast<std::chrono::nanoseconds> (d2-d1).count();
    std::cout << "Upadate adj_list elapsed: " << d_time << "ns" << std::endl;
    

    d1 = std::chrono::high_resolution_clock::now();
    for(int i=0; i < space_list.size(); i++) {
        Square* curr = space_list[i];
        curr->symbol = 1;
        dfs(1);
        curr->symbol =0;
    }
    d2 = std::chrono::high_resolution_clock::now();

    d_time = std::chrono::duration_cast<std::chrono::milliseconds> (d2-d1).count();
    std::cout << "DFS and BFS time elapsed is: " << d_time << "ms" << std::endl;

    std::cout << _max_sum << std::endl;

    end = std::chrono::high_resolution_clock::now();
    long long time  = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
    std::cout << "Total time elapsed: " << time << "ms" <<std::endl;
    return 0;


    /// =========test============ ///
    /*
    std::vector<std::vector<Square*> > result = do_combi();
    int sum;
    int max_sum = 0;

    for(int i=0; i < result.size(); i++) {
        std::vector<Square*> made_wall;
        for(int j=0; j<result[i].size(); j++) {
            Square* curr = result[i][j];
            curr->symbol = 1;
            made_wall.push_back(curr);
        }
        
        sum = bfs();
        if(sum > max_sum)
            max_sum = sum;
        
        for(int k=0; k< made_wall.size();k++) {
            Square* node = made_wall[k];
            node->symbol = 0;
        }

    }
    std::cout << max_sum << std::endl;
    */
}

