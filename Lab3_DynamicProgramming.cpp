#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iomanip>

using namespace std;

int getRndNum(int a, int b)
{
    int rndNum;
    rndNum = rand() % (b - a) + a;

    return rndNum;
}

//----------------------------------------------------------

void max_find_N()
{
    int N;

    cout << "\n  Degree (>2): ";
    cin >> N;

    vector<int> numOfOps(N + 1, 0); // number of operations

    for (int i = 2; i <= N; i++) {
        numOfOps[i] = numOfOps[i - 1] + 1;
        for (int j = 2; j <= i - 1; j++) {
            numOfOps[i] = min(numOfOps[i], numOfOps[j] + numOfOps[i - j] + 1);
            if (i % j == 0)
                numOfOps[i] = min(numOfOps[i], numOfOps[i / j] + j - 1);
        }
    }

    cout << "\n  Degree:     " << setw(3);
    for (int i = 2; i <= N; i++)
        cout << i << setw(3);
    cout << "\n  Operations: " << setw(3);
    for (int i = 2; i <= N; i++)
        cout << numOfOps[i] << setw(3);
    cout << "\n\n";
}

//----------------------------------------------------------

vector<vector<bool>> isFilled(string vert, string hor)
{
    vector<vector<bool>> fills (vert.size(), vector<bool>(hor.size(), false));

    for (int i = vert.size() - 1; i >= 0; i--)
        for (int j = 0; j < hor.size(); j++)
            if (vert[i] == hor[j])
                fills[i][j] = true;

    return fills;
}

vector<vector<int>> bioMatrixNumFill(vector<vector<bool>> fills, int M, int N)
{
    vector<vector<int>> matrix(M, vector<int>(N, 0));

    for (int i = M - 1; i >= 0; i--)
        for (int j = 0; j < N; j++)
            if (fills[i][j]) {
                if (i != M - 1 && j != 0)
                    matrix[i][j] = matrix[i + 1][j - 1] + 1;
                else
                    matrix[i][j]++;
            }
            else {
                if (i != M - 1 && j != 0)
                    matrix[i][j] = max(matrix[i + 1][j], matrix[i][j - 1]);
                else if (j == 0 && i != M - 1)
                    matrix[i][j] = matrix[i + 1][j];
                else if (i == M - 1 && j != 0)
                    matrix[i][j] = matrix[i][j - 1];
                else
                    matrix[i][j] = 0;
            }

    return matrix;
}

void maxChainFind(vector<vector<bool>> fills, int M, int N, string vert)
{
    vector<vector<int>> coords;
    bool found_hor;
    int num = 0;

    for (int i = 0; i < M; i++) {
        found_hor = false;
        for (int j = N - 1; j > 0; j--) {
            vector<int> buff(2, 0);
            if (fills[i][j] && !found_hor) {
                buff[0] = i;
                buff[1] = j;
                coords.push_back(buff);
                found_hor = true;
                num++;
            }

            if (fills[i][j - 1])
                found_hor = false;

            if (found_hor)
                break;
        }

        if (!fills[i + 1][N - 1] && fills[i][N - 1])
            break;
    }

    string DNA_str = "", buff_str;
    int buffj, chainNum, maxChain = 0;
    for (int k = 0; k < num; k++) {
        chainNum = 0;
        buff_str = "";
        buffj = coords[k][1];

        for (int i = coords[k][0]; i < M && buffj >= 0; i++) {
            if (fills[i][buffj]) {
                chainNum++;
                buff_str += vert[i];
            }

            buffj--;
        }

        if (chainNum > maxChain) {
            DNA_str = buff_str;
            maxChain = chainNum;
        }
    }

    reverse(DNA_str.begin(), DNA_str.end());
    cout << "  Max DNA chain: " << DNA_str << "\n\n";
}

void molecular_biology()
{
    string vert, hor;

    cout << "\n  DNA chain 1 (a, g, c, t): ";
    cin >> vert;
    cout << "  DNA chain 2 (a, g, c, t): ";
    cin >> hor;

    vector<vector<int>> matrix = bioMatrixNumFill(isFilled(vert, hor), vert.size(), hor.size());

    cout << "\n  ";
    for (int i = 0; i <= vert.size(); i++)
    {
        if (i != vert.size())
            cout << vert[i] << " ";
        else
            cout << "  ";

        for (int j = 0; j < hor.size(); j++)
            if (i != vert.size())
                cout << matrix[i][j] << " ";
            else
                cout << hor[j] << " ";
        cout << "\n  ";
    }

    cout << endl;
    maxChainFind(isFilled(vert, hor), vert.size(), hor.size(), vert);
}

//----------------------------------------------------------

void items_in_backpack()
{
    int maxW, numOfItems;

    cout << "\n  Number of items: ";
    cin >> numOfItems;
    cout << "  Max weight: ";
    cin >> maxW;

    vector<int> weights(numOfItems, 0), prices(numOfItems, 0);

    cout << "\n  Weights:\n  ";
    for (int i = 0; i < numOfItems; i++) {
        weights[i] = getRndNum(1, maxW);
        cout << weights[i] << " ";
    }

    cout << "\n  Prices:\n  ";
    for (int i = 0; i < numOfItems; i++) {
        prices[i] = getRndNum(1, maxW);
        cout << prices[i] << " ";
    }

    vector<vector<int>> backpack(weights.size() + 1, vector<int>(maxW + 1, 0));

    for (int i = 1; i <= weights.size(); i++)
        for (int j = 1; j <= maxW; j++)
            if (j >= weights[i - 1])
                backpack[i][j] = max(backpack[i - 1][j], backpack[i - 1][j - weights[i - 1]] + prices[i - 1]);
            else
                backpack[i][j] = backpack[i - 1][j];

    cout << "\n\n  Backpack:\n  ";
    for (int i = 1; i <= weights.size(); i++) {
        for (int j = 1; j <= maxW; j++)
            cout << backpack[i][j] << setw(3);
        cout << endl;
    }

    cout << "\n  Max price: " << backpack[weights.size()][maxW] << "\n\n";
}

//----------------------------------------------------------

vector<vector<int>> pathMatrixGen(int N)
{
    bool stairFill = 0;

    vector<vector<int>> pathMatrix(N * 2 - 1, vector<int>(N));

    cout << "\n  Original matrix:\n  ";
    for (int i = 0; i < N * 2 - 1; i++) {
        if (stairFill) stairFill = false;
        else stairFill++;
            
        for (int j = 0; j < N - stairFill; j++) {
            pathMatrix[i][j] = getRndNum(1, 10);
            cout << pathMatrix[i][j] << " ";
        }
        cout << "\n  ";
    }

    return pathMatrix;
}

void getResultMatrix(vector<vector<int>> pathMatrix)
{
    int N = pathMatrix[1].size();

    vector<vector<int>> resultMatrix(N, vector<int>(N, 0));

    for (int j = N - 2; j >= 0; j--)
        resultMatrix[0][j] = resultMatrix[0][j + 1] + pathMatrix[0][j];

    for (int i = 1; i < N; i++) {
        resultMatrix[i][N - 1] = resultMatrix[i - 1][N - 1] + pathMatrix[2 * i - 1][N - 1];

        for (int j = N - 2; j >= 0; j--)
            resultMatrix[i][j] = min(resultMatrix[i - 1][j] + pathMatrix[2 * i - 1][j],
                resultMatrix[i][j + 1] + pathMatrix[2 * i][j]);
    }

    cout << "\n  Total matrix:\n  ";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << resultMatrix[i][j] << " ";
        cout << "\n  ";
    }

    cout << "\n  Min path: " << resultMatrix[N - 1][0] << endl;
}

void reverseBypass(vector<vector<int>> pathMatrix)
{
    int N = pathMatrix[1].size(), i = N - 1, j = 0;
    vector<int> buff(2);
    vector<vector<int>> backPathIndexes;

    buff[0] = i;
    buff[1] = j;
    backPathIndexes.push_back(buff);

    while (i > 0 || j < N - 1)
    {
        if (i == 0)
            j++;
        else if (j == N - 1)
            i--;
        else {
            if (pathMatrix[2 * i - 1][j] < pathMatrix[2 * i][j])
                i--;
            else
                j++;
        }

        buff[0] = i;
        buff[1] = j;
        backPathIndexes.push_back(buff);
    }

    cout << "\n  Reverse bypass:\n  i j\n  ";
    for (int i = 0; i < backPathIndexes.size(); i++) {
        for (int j = 0; j < 2; j++)
            cout << backPathIndexes[i][j] << " ";
        cout << "\n  ";
    }
}

void optimal_path()
{
    int N;

    cout << "\n  Size of square matrix: ";
    cin >> N;

    vector<vector<int>> pathMatrix = pathMatrixGen(N);
    getResultMatrix(pathMatrix);
    reverseBypass(pathMatrix);

    cout << endl;
}

//----------------------------------------------------------

vector<vector<int>> triangleMatrixGen(int N)
{
    vector<vector<int>> triangleMatrix(N, vector<int>(N, 0));

    cout << "\n  Original matrix:\n  ";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            triangleMatrix[i][j] = getRndNum(1, 10);
            cout << triangleMatrix[i][j] << " ";
        }
        cout << "\n  ";
    }

    return triangleMatrix;
}

vector<vector<int>> getSumMatrix(vector<vector<int>> triangleMatrix)
{
    int N = triangleMatrix.size();

    vector<vector<int>> sumMatrix(N, vector<int>(N + 1, 0));

    for (int i = 0; i < N; i++)
        for (int j = 1; j <= N; j++)
            sumMatrix[i][j] = triangleMatrix[i][j - 1];

    cout << "\n  Total matrix:\n  " << sumMatrix[0][1] << " \n  ";
    for (int i = 1; i < N; i++) {
        for (int j = 1; j <= i + 1; j++) {
            sumMatrix[i][j] = sumMatrix[i][j] + max(sumMatrix[i - 1][j - 1],
                sumMatrix[i - 1][j]);
            cout << sumMatrix[i][j] << " ";
        }
        cout << "\n  ";
    }

    return sumMatrix;
}

void reverseBypassTriangle(int N)
{
    vector<vector<int>> sumMatrix = getSumMatrix(triangleMatrixGen(N));

    int maxEl = 0, maxj;
    for (int j = 1; j <= N; j++)
        if (maxEl < sumMatrix[N - 1][j])
        {
            maxEl = sumMatrix[N - 1][j];
            maxj = j;
        }

    cout << "\n  Maximum element: " << maxEl << endl;

    cout << "\n  Reverse bypass:\n  i j\n  " << N - 1 << " " << maxj - 1 << endl;
    for (int i = N - 2; i >= 0; i--) {
        if (sumMatrix[i][maxj - 1] > sumMatrix[i][maxj])
            maxj--;
        cout << "  " << i << " " << maxj - 1 << endl;
    }
    cout << endl;
}

void triangle()
{
    int N;

    cout << "\n  Size of triangle matrix: ";
    cin >> N;

    reverseBypassTriangle(N);
}

//----------------------------------------------------------

void algorhitmChoise()
{
    int n = 0, answer = 0;

    cout << "//----------------------------------------------------------\n\n";

    cout << "Choose Algorhytm.\n 1. Min. number of operations for exponation.\n";
    cout << " 2. molecular_biology.\n 3. items_in_backpack\n";
    cout << " 4. optimal_path\n 5. triangle\nYour choice: ";
    cin >> answer;

    switch (answer) {
    default:
        return;
        break;
    case 1:
        max_find_N();
        break;
    case 2:
        molecular_biology();
        break;
    case 3:
        items_in_backpack();
        break;
    case 4:
        optimal_path();
        break;
    case 5:
        triangle();
        break;
    }

    algorhitmChoise();
}

int main()
{
    srand(time(NULL));

    algorhitmChoise();
}