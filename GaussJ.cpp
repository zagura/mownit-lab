#include <iostream>
#include <vector>
#include <utility>


using namespace std;
namespace mz{
	struct Matrix{
		Matrix(int n){
			this->rows = n;
			this->columns = n;
			this->v = vector <vector <double> >(n);
			for(int i = 0 ; i < n ; i++){
				std::vector<double> row = std::vector<double>(n);
				v[i] = (row);
			}
		}
		Matrix(int r, int c){
			this->rows =  r;
			this->columns = r;
			this->v = vector< vector<double> >(r);
			for(int i = 0; i < r; i++){
				vector<double> vec(c);
				v[i] = vec;
			}
		}
		Matrix(void){
			this->rows = 0;
			this->columns = 0;
			this->v = vector<vector<double> >(0);
		}
		Matrix(const Matrix& M){
			this->rows = M.rows;
			this->columns = M.columns;
			this->v = vector<std::vector<double> > (M.v);
		}
		void print(void){
			cout<<"Matrix:\n";
			for(int r = 0; r < rows; r++){
				for(int c = 0; c < columns; c++){
					cout<<this->v[r][c]<<"\t";
				}
				cout<<endl;
			}
			cout<<"----------------------------------\n\n";
		}
		vector < vector <double> >  v;
		int rows;
		int columns;
	};

	struct pivot{
		Matrix* M;
		int row;
		int column;
		double val;
	};

	pivot find_pivot(Matrix* M, int row_begin, int col_begin){
		// cout<<"FIND PIVOT"<<endl;
		pivot p;
		p.M = M;
		p.row = row_begin;
		p.column = col_begin;
		p.val = 0.0;
		double max_val = M->v[row_begin][col_begin];
		for(int i = row_begin; i < M->rows; i++){			// For partial pivot i = row_begin and there isn't this loop
			for(int j = col_begin; j < M->columns; j++){
				if(M->v[i][j] > max_val){
					max_val = M->v[i][j];
					p.row = i;
					p.column = j;
				}
			}
		}
		p.val = max_val;
		return p;
	}

	pivot find_partial_pivot(Matrix* M, int row_begin, int column){
		pivot p;
		p.M = M;
		p.row = row_begin;
		p.column = column;
		double max_val = M->v[row_begin][column];
		for(int r = row_begin + 1; r < M->rows; r++){
			if(M->v[r][column] > max_val){
				max_val = M->v[r][column];
				p.row = r;
			}
		}
		p.val = max_val;
		return p;
	}

	void swap_columns(Matrix& M, int first, int second, std::vector<double*>& variables){
		int rows = M.rows;
		double tmp = 0.0;

		for(int i = 0; i < rows; i++){
			tmp = M.v[i][first];
			M.v[i][first] = M.v[i][second];
			M.v[i][second] = tmp;
		}
		double* tmp_var = variables[first];
		variables[first] = variables[second];
		variables[second] = tmp_var;

	}

	void swap_rows(Matrix& M, int first, int second, vector <double>& results){
		vector<double> tmp_vec = M.v[first];
		M.v[first] = M.v[second];
		M.v[second] = tmp_vec;
		double tmp_res = results[first];
		results[first] = results[second];
		results[second] = tmp_res;
	}

	void reduce(vector<double> reducing_row, vector<double>& reduced_row, pivot p, double res, double& result){
		//	cout<<"REDUCE"<<endl;
		double division = reduced_row[p.column] / reducing_row[p.column];
		for(int i = 0; i < reducing_row.size(); i++){
			reduced_row[i] -= reducing_row[i] * division;
		}
		result -= res * division;
	}

	void solve_triangle_up(Matrix& M, vector <double*>& variables, vector<double>& results){
		// cout<<"SOLE TRAINGLE UP \n\n";
		for(int i = (M.rows - 1); i >= 0; i--){
			double a = M.v[i][i];
			double b = results[i];
			for(int j = i-1; j >= 0; j--){
				double division = M.v[j][i] / a;
				M.v[j][i] = 0;
				results[j] -= division * b;
			}
			*(variables[i]) = b / a;
		}
	}

	void solve_triangle_down(Matrix& M, vector <double*>& variables, vector<double>& results){
		for(int i = 0; i < M.rows; i++){
			double a = M.v[i][i];
			double b = results[i];
			for(int j = i + 1; j < M.rows ;j++){
				double division = M.v[j][i] / a;
				M.v[j][i] = 0;
				results[j] -= division * b;
			}
			*(variables[i]) = b / a;
			// cout<<b/a;
		}
	}

	void solve(Matrix& M, vector<double*>& variables, vector<double>& results){
		int n = M.rows;
		if(M.columns < n) n = M.columns;
		for(int i = 0; i < n; i++){
			pivot p = find_pivot(&M, i, i);
			if(p.column > i){
				swap_columns(M, i, p.column, variables);
				p.column = i;
			}
			if(p.row > i){
				swap_rows(M, i, p.row, results);
				p.row = i;
			}
			//
			M.print();
			//
			for(int j = i + 1; j < M.rows; j++){
				reduce(M.v[i], M.v[j], p, results[i], results[j]);
			}
			M.print();
		}
		solve_triangle_up(M, variables, results);
	}

	Matrix multiply(Matrix& left, Matrix& right){
		int rows = left.rows;
		int columns = right.columns;
		int n = left.columns;
		if(n != right.rows){
			throw "Wrong arguments to multiply matrixex";
		}
		Matrix res(rows, columns);
		for(int r = 0; r < rows; r++){
			for(int c = 0; c < columns; c++){
				double val = 0.0;
				for(int i = 0; i < n; i++){
					val += left.v[r][i] * right.v[i][c];
				}
				res.v[r][c] = val;
			}
		}
		return res;
	}

	vector <double> multiply(Matrix& left, vector<double> vec){
		if(left.columns != vec.size()) throw "Wrong arguments sizes";
		vector<double> res(vec.size());
		for(int r = 0; r < vec.size(); r++){
			double val = 0.0;
			for(int c = 0; c < left.columns; c++){
				val += left.v[r][c] * vec[c];
			}
			res[r] = val;
		}
		return res;
	}

	Matrix get_transpose(const Matrix &a){
		Matrix t(a.columns, a.rows);
		for(int r = 0; r < a.columns; r++){
			for(int c = 0; c < a.rows; c++){
				t.v[r][c] = a.v[c][r];
			}
		}
		return t;
	}

	struct LU_dec{
		LU_dec(const Matrix& L, const Matrix& U, vector<double> b){
			this->L = L;
			this->U = U;
			this->b = b;
		}
		mz::Matrix L;
		mz::Matrix U;
		std::vector<double> b;
	};

	LU_dec* LU_decomposition(Matrix& A, vector<double*>& x, vector<double> b){
		Matrix L = (A.rows, A.columns);
		Matrix U = A;
		vector<double> v = b;
		int n = A.columns;
		for(int i = 0;  i < n; i++){
			pivot p = find_partial_pivot(&U, i, i);
			if(p.val == 0) throw "A Matrix is singular";
			if(p.row != i){
				swap_rows(U, i, p.row, b);
				swap_rows(L, i, p.row, v);
			}
			L.v[i][i] = 1;
			for(int k = i+1; k < n; k++){
				L.v[k][i] = U.v[k][i] / U.v[i][i];
				for(int l = i + 1; l < n; l++){
					U.v[k][l] = U.v[k][l] - L.v[k][i]*U.v[i][l];
				}
			}
		}
		for(int r = 0 ; r < n ; r++){
			for(int c = 0 ; c < r ; c++){
				U.v[r][c] = 0;				//Cleanup of lower part in U matrix
			}
		}
		LU_dec* result = new LU_dec(L, U, b);
		L.print();
		U.print();
		return result;
	}

	void solve_lu_method(Matrix& A, vector<double*>& x, vector<double>& b){
		vector<double> y(x.size());
		vector<double*> vars(x.size());
		for(int i = 0; i < x.size(); i++){
			vars[i] = &(y[i]);
		}
		LU_dec* lu = LU_decomposition(A,x,b);
		solve_triangle_down(lu->L, vars, lu->b);
		solve_triangle_up(lu->U , x, y);
	}

	void solve_normalized_LU(Matrix& A, vector <double*>&x, vector<double>& b){
		Matrix A_t = get_transpose(A);
		Matrix left = multiply(A_t, A);
		vector<double> right = multiply(A_t, b);
		solve_lu_method(left, x, right);
	}

}
int main(void){
	try{
		int n = 0;
		cin>>n;
		double* vars = new double[n];
		for(int i = 0; i < n; i++){
			vars[1] = 0.0;
		}
		mz::Matrix A(n);
		vector<double*> res;
		vector<double> b(n);
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				cin>>A.v[i][j];
			}
			res.push_back(&(vars[i]));
			cout<<"Right side: ";
			cin>>b[i];
		}
		A.print();
		//mz::solve_lu_method(A, res, b);
		mz::solve_normalized_LU(A, res, b);
		for(int i = 0; i < n; i++){
		 	cout<<i<<": "<<vars[i]<<endl;
		}
		// mz::LU_dec* lu = mz::LU_decomposition(A, res, b);
		//delete lu;
		delete[] vars;
		
		return 0;
	}catch(string c){
		cout<<"Error: "<<c<<endl;
		return 1;
	}
}