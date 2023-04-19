//
// 	SimpleTester.cpp - main file of a project
//
// 	2022-2023	Damian Strojek @damianStrojek
//
// 	The program is made for students who want to learn abcd questions for the exam
//	For it to work you need to provide a .txt database with properly formatted questions
//
// 	g++ simple-tester.cpp -o simple-tester
//
// 	Project realised in years 2022-2023 on
// 	Gdansk University of Technology
// 

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>

#define LIMITCORRECT 500
#define ERRORFILE 0
#define EXIT 1
#define ERRORANSWER 2

struct answer {
	std::string desc;
	bool isCorrect;
};

class Question {
private:
	std::string question;			// Question itself
	std::vector<answer> answers;		// All of the answers
	int numberOfCorrectAnswers;		// Number of correct answers
	std::string correctAnswer;		// Array of correct answers
	int answeredCorrectly;			// How many times you answered this question correctly (basis for sorting)
	bool openQuestion;			// Check if this question is open
public:
	Question(std::string _question) : question(_question), answeredCorrectly(0), numberOfCorrectAnswers(0), openQuestion(false) {}
	std::string getQuestion() { return this->question; }
	std::vector<answer> &getAnswers() { return this->answers; } 
	answer getAnswerIndex(const int index) { return this->answers[index]; }
	int getAnsweredCorrectly() { return this->answeredCorrectly; }
	std::string getCorrectAnswer() { return this->correctAnswer; }
	bool isOpen(){ return this->openQuestion; }
	int getNumberCorrect() { return this->numberOfCorrectAnswers; }

	void setCorrectAnswerIndex(const int index) { 
		this->answers[index].isCorrect = true; 
		this->correctAnswer += (char)(index + 'a');
		this->numberOfCorrectAnswers++;
	}

	void setQuestion(std::string _question) { this->question = _question; }

	void setOpen() { this->openQuestion = true; }

	void addAnswer(std::string _answer) {
		answer temp;
		temp.desc = _answer;
		
		// If question is open then there is only one correct answer
		if(this->openQuestion) {
			temp.isCorrect = true;
			this->correctAnswer = _answer;
		}
		else temp.isCorrect = false;

		this->answers.push_back(temp);
	}

	void addAnsweredCorrectly() { this->answeredCorrectly++; }

	void reorganizeAnswers() { std::random_shuffle(std::begin(this->answers), std::end(this->answers)); }

	void displayCorrectAnswers(){
		if(this->openQuestion)
			std::cout << "\n\t[WRONG] Correct answer:\n\t\t" << this->correctAnswer;
		else {
			std::cout << "\n\t[WRONG] Correct answers:";
			for(int i = 0; i < this->numberOfCorrectAnswers; i++){
				int answerIndex = (int)correctAnswer[i] - 97;
				std::cout << "\n\t\t" << this->answers[answerIndex].desc;
			}
		}
	};
};

int sortQuestions(Question, Question);
void invokeExit(int);
void helloMessage();
std::ifstream loadFile();
void loadQuestionsAnswers(std::ifstream&, std::vector<Question>&, std::string, int&);
void displayQuestions(int&, int&, std::vector<Question>&, const bool&, const int&);
void checkQuestions(std::vector<Question>&);
void countQuestions(std::vector<Question>&);

int main(){

	helloMessage();
	std::srand(unsigned(std::time(0)));
	std::ifstream databaseFile = loadFile();
	std::vector<Question> questions;
	std::string temp;
	int numberOfQuestions = 0;

	while(databaseFile.good()){
		std::getline(databaseFile, temp);
		if(!temp.empty()) loadQuestionsAnswers(databaseFile, questions, temp, numberOfQuestions);
	}

	std::cout << "\n\tNumber of questions loaded from the file: " << numberOfQuestions 
			<< "\n\tDo you want to see number of correct answers for each question? [y/n] ";
	bool seeCorrect = false;
	std::cin >> temp;
	if (temp == "y") seeCorrect = true;

	std::cout << "\n\tPress ENTER if you are ready for the first question. ";
	std::cin.ignore();

	// Had to come up with something better than while(true)
	// Two variables needed to keep track of statistics
	int correctQuestions = 0, allQuestionsAsked = 0;
	while(correctQuestions < LIMITCORRECT)
		displayQuestions(correctQuestions, allQuestionsAsked, questions, seeCorrect, numberOfQuestions);

	invokeExit(EXIT);
	return 0;
};

int sortQuestions(Question _question1, Question _question2) {
	return _question1.getAnsweredCorrectly() < _question2.getAnsweredCorrectly();
};

void invokeExit(int status) {
	if (!status) std::cout << "\n\t[ERROR] Loading of the file failed.\n";
	else if (status) std::cout << "\n\t[GOODBYE] Thank you for using SimpleTester.\n";
	else if (status == 2) std::cout << "\n\t[ERROR] Loading of an answer failed.\n";
	
	// Pause for end user and exit the program
	std::cin.ignore();
	exit(1);
};

void helloMessage(){
	//system("cls");	// Windows version
	system("clear");	// Linux version
	std::cout << "\n\n\tSimpleTester release 3.1\n\n\tContributors: damianStrojek@2023" << 
		", Lemm@2012, Pitya@2022\n\n\tHave fun learning. Fuck GUT.\n\n";
	return;
};

std::ifstream loadFile(){
	std::cout << "\t\tInsert name of the database file [ANSI encoded]: ";
	std::string databaseName;
	std::cin >> databaseName;

	std::ifstream inputFile;
	inputFile.open(databaseName, std::ios::in);
	if(!inputFile.good()) invokeExit(ERRORFILE);

	return inputFile;
};

void loadQuestionsAnswers(std::ifstream &databaseFile, std::vector<Question> &questions, 
				std::string _question, int &numberOfQuestions){
	
	
	// Load all of the answers
	std::string temp;
	std::getline(databaseFile, temp);
	Question newQuestion(_question);
	// Check whether this is an open or abcd question
	if(temp == "o"){
		std::getline(databaseFile, temp);
		newQuestion.setOpen();
		newQuestion.addAnswer(temp);
	}
	else {
		int numberOfAnswers = temp[0] - '0', i, index;
		for(i = 0; i < numberOfAnswers && databaseFile.good(); i++){
			std::getline(databaseFile, temp);
			newQuestion.addAnswer(temp);
		}
		
		// Load the correct answer
		if(i == numberOfAnswers){
			std::getline(databaseFile, temp);
			// This could be numerical (answers in source files)
			// but I would have to reformat all databases
			for(int j = 0; j < temp.size(); j++){
				if(temp[j] > 96 && temp[j] < 105){
					index = (int)temp[j] - 97;
					newQuestion.setCorrectAnswerIndex(index);
				}
			}
		}
	}

	questions.push_back(newQuestion);
	numberOfQuestions++;
};

void displayQuestions(int &correctQuestions, int &allQuestionsAsked, std::vector<Question> &questions, 
			const bool &seeCorrect, const int &numberOfQuestions){
	//system("cls");	// Linux version
	system("clear");	// Windows version

	std::string answer;
	bool answeredCorrectly = false;
	std::cout << "\n\tYour actual score: " << (double)((double)correctQuestions / 
		(double)(allQuestionsAsked == 0 ? 1 : allQuestionsAsked)) * 100.0f << 
		"% (" << correctQuestions << "/" << allQuestionsAsked << ")\n\n";

	// Sorting Mechanism for Questions and selecting next question
	if (!(correctQuestions % numberOfQuestions)) std::random_shuffle(std::begin(questions), std::end(questions));
		else sort(questions.begin(), questions.end(), sortQuestions);
	Question activeQuestion = questions.front();

	// Print next question and answer
	if(activeQuestion.isOpen()) {
		std::cout << "\n\t[OPEN QUESTION] " << activeQuestion.getQuestion() << "\n\n\tYour answer: ";
		std::getline(std::cin, answer);

		// Compare user answer with the correct answer
		if(activeQuestion.getAnswers()[0].desc == answer) answeredCorrectly = true;
	}
	else {
		if(seeCorrect) 
			std::cout << "\n\t[" << activeQuestion.getNumberCorrect() << " correct answers] " << activeQuestion.getQuestion();

		int numberOfAnswers = activeQuestion.getAnswers().size();
		
		// Shuffle answers and display
		activeQuestion.reorganizeAnswers();
		for(int i = 0; i < numberOfAnswers; i++) 
			std::cout << "\n\t\t" << (char)(i + 'a') << ". " << activeQuestion.getAnswers()[i].desc;
		
		std::cout << "\n\n\tYour answer: ";
		std::getline(std::cin, answer);

		// Compare user answer with the correct answer
		for(int i = 0; i < answer.size(); i++){
			if(answer[i] > 96 && answer[i] < 105){
				int answerIndex = (int)answer[i] - 97;
				// If index is out of scope
				if(answerIndex >= activeQuestion.getAnswers().size()) invokeExit(ERRORANSWER);
				// If this part of answer is correct we continue checking
				else if(activeQuestion.getAnswers()[answerIndex].isCorrect){
					answeredCorrectly = true;
					continue;
				}
				// Else we break from the for loop and have the wrong answer
				else {
					answeredCorrectly = false;
					break;
				}
			}
		}
	}
	allQuestionsAsked++;

	// Output if the user was right or not
	if(answeredCorrectly)
		std::cout << "\n\t\t[CORRECT] Well done!";
		activeQuestion.addAnsweredCorrectly();
		correctQuestions++;
	else activeQuestion.displayCorrectAnswers();

	std::cout <<"\n\n\t[NEXT QUESTION] Press ENTER, CHECK/COUNT or EXIT: ";
	std::getline(std::cin, answer);
	if(answer == "check" || answer == "CHECK")
		// Check the database file
		checkQuestions(questions);
	else if(answer == "count" || answer == "COUNT")
		// Prints how many times each question have been answered correctly
		countQuestions(questions);
	else if(answer == "exit" || answer == "EXIT")
		// Exit the program
		invokeExit(EXIT);
};

// Below functions are clearly to debbug and check the database file while writing it
void checkQuestions(std::vector<Question> &questions) {
	std::cout << "\n\n\t[QUESTIONS]";
	int i;
	for (i = 0; i < questions.size(); i++) {
		std::cout << "\n\t" << questions[i].getQuestion();
		for (int j = 0; j < questions[i].getAnswers().size(); j++)
			std::cout << "\n\t\t" << (char)(j + 'a') << ". " << questions[i].getAnswerIndex(j).desc << 
				"\t[CORRECT y/n] " << questions[i].getAnswerIndex(j).isCorrect;
	}

	std::cout << "\n\n\t[ANSWERS ONLY]\n";
	for(i = 0; i < questions.size(); i++)
		std::cout << "\n\t\t[QUESTION " << i+1 << "] Correct answer: " << questions[i].getCorrectAnswer();
	std::cin.ignore();
};

void countQuestions(std::vector<Question> &questions){
	std::cout << "\n\n\t[QUESTIONS]";
	for (int i = 0; i < questions.size(); i++) {
		std::cout << "\n\t" << questions[i].getQuestion() << "\t[TIMES: " << 
			questions[i].getAnsweredCorrectly() << "]\n"; 
	}
	std::cin.ignore();
};