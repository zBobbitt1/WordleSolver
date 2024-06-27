#include <iostream>	
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

//converts character to number within alphabet ie a=1, b=2 ...
int returnVal (char x)
{
    return (int) x - 97;
}

//If "guess" is guessed and "ansewer" is answer will "checked" be remove
bool removeWord(string guess, string answer, string checked){
    bool removed =  false;
    int i = 0;
    
    while(i<5 && !removed){
        int result; //result=0 if black, 1 if yellow, 2 if green
        int j = 0;

        while(j<5 && !removed){

            if(guess.at(i) == answer.at(j)){

                if(i==j && returnVal(checked.at(i)) != returnVal(guess.at(i))){
                    removed = true;
                    break;
                }

                else if(i!=j && returnVal(checked.at(0)) != returnVal(guess.at(i)) && returnVal(checked.at(1)) != returnVal(guess.at(i)) && returnVal(checked.at(2)) != returnVal(guess.at(i)) && returnVal(checked.at(3)) != returnVal(guess.at(i)) && returnVal(checked.at(4)) != returnVal(guess.at(i))){
                    removed = true;
                    break;
                }

            }

            j++;
        }

        i++;
    }
    
    return removed;
};

//If "guess" is guessed and outPut is of the form GGBYY or BBBYG is "checked" kept?
bool isKept(string guess, string outPut, string checked){
    bool kept = true;
    int i = 0;

    while(i<5 && kept){
        char a = outPut.at(i);  

        if(a == 'G' && guess.at(i) != checked.at(i)){
            kept = false;
            break;
        }  

        else if(a == 'Y'){

            if(guess.at(i) == checked.at(i)){
                kept = false;
                break;
            }

            bool yellow[4];
            for(int m = 0; m<4; m++){
                yellow[m] = true;
            }

            int j = 0;
            for(int k = 0; k < 5; k++){
                if(k != i && guess.at(i) != checked.at(k)){
                    yellow[j] = false;
                    j++; 
                }
            }

            if(!yellow[0] && !yellow[1] && !yellow[2] && !yellow[3]){
                kept = false;
                break;
            }



        }

        else if(a == 'B'){
            for(int k = 0; k<5; k++){
                if(guess.at(i) == checked.at(k)){
                    kept = false;
                    break;
                }
            }
        }

        else{}

        i++;
        
    }
    return kept;

}

//Each Node represents a single word
class Node{

    public:
    string word;  //Each Node contains a Player Object
    Node *pNext;  //A Pointer to the next Node
    Node *pPrev;  //A pointer to the previous Node

    //Default Constructor creates new Node 
    Node(const string& e){
	word = e;      
	pNext = NULL;  //Pointers to NULL before Node before added to Link List
    pPrev = NULL;
    };

};

class wordList{

    Node* pFirst;    //Points to first Node in Link List   
	Node* pLast;     //Points to last Node in Link List
    Node* pCurrent;  //Points to node that iteration is currently at

	int   m_count;   //Tracks total Nodes in Link List

    int ocrArray[5][26];  //tracks number of letter occurances

    public:

    //default constuctor
    wordList(){

        m_count = 0;     //List starts with 0 items		    
	    pFirst = NULL;	 //All pointer to Null with no existing Nodes				
	    pLast = NULL;
	    pCurrent = NULL;

        //iterate through every occurance value, set to 0
        for(int i = 0; i<5; i++){
            for(int j = 0; j<26; j++){
                ocrArray[i][j] = 0;
            }
        }

    }

    //Link List is empty when PFirst is NULL
    bool isEmpty() {
	    return pFirst == NULL;   
    }

    //Link List is never full
    bool isFull() {
	    return false;   
    }

    //Does LL have Next
    bool hasNext() {
	    return pCurrent != NULL; 
    }

    //Get next string 
    string getNext() {
	    string e;
	    if (hasNext()) {                     // Checks if next Node exists
		    e = pCurrent->word;              // Assigns player to current item
		    pCurrent = pCurrent->pNext;      // iterate pCurrent to next Node
	    }
	    return e;
    }

    //Reset pCurrent
    void resetIteration() {
	    pCurrent = pFirst;
    }

    //Return Length of list
    int getLength(){
        return m_count;
    }

    //Adds a node given string
    bool add(const string& e) {

        Node* p;	      //Create Node with player e and pointer p 	
        p = new Node(e); 

        //If no Nodes exist point pFirst, pLast, pCurrent to p
	    if (pFirst == NULL) { 
		    pFirst = p;
		    pLast = p;
		    pCurrent = p;     
	    }

        //If Nodes do exist link p to pLast and incremint pLast to p
	    else { 
	    	pLast->pNext = p;
            p->pPrev = pLast;
		    pLast = p;
	    }

	    m_count++;    //Add one to size of list    
	    return true;  //Add was successful    
    }

   //finds proper letter distribution given any totalList of words
    void calcOcrArray(){

        resetIteration();

        //iterate through every letter of every word in List
        while(hasNext()){
            string a = getNext();
            for(int i = 0; i<5; i++){
                ocrArray[i][returnVal(a.at(i))]++;
            }
        }

        resetIteration();

    }

    //Get occurance of letter b at position a
    int getOcrValue(int a, int b){
        return ocrArray[a][b];
    }

    //the score of a string a equals four times the number of times a letter in a appears in our list a (in the same position) 
    //plus the number of times that letter appears in our list (not in the same position)
    int scoreCalc(string a, int x, int y){ //ints are solved and found
        int score = 0;
        //iterate through every letter summing the total score for both same position score and different position
        for(int i = 0; i<5; i++){

            bool sameLet = false;
            for(int k = 0; k<i; k++){
                if(returnVal(a.at(i)) == returnVal(a.at(k))){
                    sameLet = true;
                }
            }

            score = score + (4)*(getOcrValue(i, returnVal(a.at(i))));
            if(!sameLet){
                for(int j = 0; j<5; j++){
                    if(i != j){
                        score = score + getOcrValue(j, returnVal(a.at(i)));
                    }
                }
            }


        }

        return score-20;    
    };

    //Takes a list of words and sorts them into this link list based on scanned list's occurance array
    bool sortList(wordList x){

        x.resetIteration();
        while(x.hasNext()){
            Node *pNew;		
	        Node *p, *q;
            pNew = new Node(x.getNext());

            p = pFirst;
	        q = NULL;

            while(p != NULL && x.scoreCalc(pNew->word, 0, 0) < x.scoreCalc(p->word, 0, 0)){
                q = p;
		        p = p->pNext;
            }

            if (pFirst == NULL && pLast == NULL) { // simplest case, this new node is going in empty list
		        pFirst = pNew;
		        pLast = pNew;
	        }
	        else if (p == NULL) {  // this case we walked off the end, so new node goes last
		        pLast->pNext = pNew;
		        pLast = pNew;
	        }
	        else if (q == NULL) { // this case our new node is going in the front
		        pNew->pNext = pFirst;
		        pFirst = pNew;
	        }
	        else {  // this case our new node goes between q and p
		        q->pNext = pNew;
		        pNew->pNext = p;
	        }
	
	        m_count++;        // count it
            
        }
        return true;
    }

    //Only used if list is sorted/ picks top word that maximizes eliminated word from the original set
    string bestGuess(wordList check, int totalChecked){
        
        string topPicks[totalChecked];
        int scores[totalChecked];

        resetIteration();
        for(int i = 0; i < totalChecked; i++){
            topPicks[i] = getNext(); 
            scores[i] = 0;  
        }

        for(int i = 0; i<totalChecked; i++){
            check.resetIteration();

            for(int j = 0; j < check.getLength(); j++){
                resetIteration();
                string k = check.getNext();

                for(int l = 0; l < getLength(); l++){
                    if(removeWord(topPicks[i], k, getNext())){
                        scores[i]++;
                    }
                }

            }
        }

        int highscore = 0;

        for(int i = 0; i < totalChecked; i++){
            if(scores[i]>scores[highscore]){
                highscore = i;
            }
        }

        return topPicks[highscore];
       
    };

    void removeInvalWord(string guess, string outPut){
        char a = outPut.at(0);
        char b = outPut.at(1);
        char c = outPut.at(2);
        char d = outPut.at(3);
        char e = outPut.at(4);

        resetIteration();

        while(hasNext()){
            string l = getNext(); 

            

        }
    }

};

int main(){

    
    ifstream inputFile("totalWordList.txt");
    wordList originList;

    string wordInput;
    while (inputFile >> wordInput) {
        originList.add(wordInput);
    }

    originList.calcOcrArray();

    wordList orderedList1;
    orderedList1.sortList(originList);

    orderedList1.resetIteration();

    for(int i = 0; i < 20; i++){
        string s = orderedList1.getNext();
        cout << i+1 << ". "<< s << endl;
    }

    cout << endl;
    cout << "The best choice is " << orderedList1.bestGuess(originList, 20) << endl;
    

//   cout << isKept("esdet", "YYBGB", "spren");

};