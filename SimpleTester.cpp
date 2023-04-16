// Simple Tester v3.0
// Damian Strojek s184407
// Refactored this code in 04.2023
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <ctime>
#include <algorithm>
#include <random>

#define LIMITCORRECT 200
#define QUESTIONINDEX 0

struct answer {
	std::string desc;
	bool isCorrect;
};

class Question {
private:
	std::string question;			// Question itself
	std::vector<answer> answers;	// All of the answers
	int numberOfCorrectAnswers;		// Number of correct answers
	std::string correctAnswer;		// Array of correct answers
	int answeredCorrectly;			// How many times you answered this question correctly (basis for sorting)
public:
	Question(std::string _question) : question(_question), answeredCorrectly(0), numberOfCorrectAnswers(0) {}
	
	std::string getQuestion() { return this->question; }
	std::vector<answer> &getAnswers() { return this->answers; } 
	answer getAnswerIndex(const int index) { return this->answers[index]; }
	int getAnsweredCorrectly() { return this->answeredCorrectly; }
	std::string getCorrectAnswer() { return this->correctAnswer; }
	int getNumberCorrect() { return this->numberOfCorrectAnswers; }

	void setCorrectAnswerIndex(const int index) { 
		this->answers[index].isCorrect = true; 
		this->correctAnswer += (char)(index + 'a');
		this->numberOfCorrectAnswers++;
	}

	void setQuestion(std::string _question) { this->question = _question; }

	void addAnswer(std::string _answer) {
		answer temp;
		temp.desc = _answer;
		temp.isCorrect = false;
		this->answers.push_back(temp);
	}

	void addAnsweredCorrectly() { this->answeredCorrectly++; }

	void reorganizeAnswers() { std::random_shuffle(std::begin(this->answers), std::end(this->answers)); }
};

int sortQuestions(Question _question1, Question _question2);
int invokeError(int status);
void helloMessage();
std::ifstream loadFile();
void loadQuestionsAnswers(std::ifstream &databaseFile, std::vector<Question> &questions,
							 std::string _question, int &numberOfQuestions);
void displayQuestions(int &correctQuestions, int &allQuestionsAsked, std::vector<Question> &questions, 
						const bool &seeCorrect, const int &numberOfQuestions);
void displayCorrectAnswers(const int index, std::vector<Question> &questions);
void checkQuestions(std::vector<Question> &questions);

int main(int argc, char* argv[]) {

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

	//checkQuestions(questions);

	std::cout << "\n\tNumber of questions loaded from the file: " << numberOfQuestions 
			<< "\n\tDo you want to see number of correct answers for each question? [y/n] ";
	bool seeCorrect = false;
	std::cin >> temp;
	if (temp == "y") seeCorrect = true;

	std::cout << "\n\tPress ENTER if you are ready for the first question. ";
	std::getline(std::cin, temp);

	// Had to come up with something better than while(true)
	// Two variables needed to keep track of statistics
	int correctQuestions = 0, allQuestionsAsked = 0;
	while(correctQuestions < LIMITCORRECT)
		displayQuestions(correctQuestions, allQuestionsAsked, questions, seeCorrect, numberOfQuestions);

	invokeError(1);
	return 0;
};

int sortQuestions(Question _question1, Question _question2) {
	return _question1.getAnsweredCorrectly() < _question2.getAnsweredCorrectly();
};

int invokeError(int status) {
	if (!status) std::cout << "\n\t[ERROR] Loading of the file failed.\n";
	else if (status) std::cout << "\n\t[GOODBYE] Thank you for using SimpleTester.\n";
	else if (status == 2) std::cout << "\n\t[ERROR] Loading of an answer failed.\n";
	
	// Pause for end user
	std::string stop;
	getline(std::cin, stop);

	exit(1);
	return status;
};

void helloMessage(){
	//system("cls");	// Windows version
	system("clear");	// Linux version
	std::cout << "\n\n\tSimpleTester release 3.0\n\n\tContributors: damianStrojek@2023" << 
					", Lemm@2012, Pitya@2022\n\n\tHave fun learning. Fuck GUT.\n\n";
	return;
};

std::ifstream loadFile(){
	std::cout << "\t\tInsert name of the database file [ANSI encoded]: ";
	std::string databaseName;
	std::cin >> databaseName;

	std::ifstream inputFile;
	inputFile.open(databaseName, std::ios::in);
	if(!inputFile.good()) invokeError(0);

	return inputFile;
};

void loadQuestionsAnswers(std::ifstream &databaseFile, std::vector<Question> &questions, 
							std::string _question, int &numberOfQuestions){
	
	// Load all of the answers
	std::string temp;
	std::getline(databaseFile, temp);
	Question newQuestion(_question);
	int numberOfAnswers = temp[0] - '0', i, index;
	for(i = 0; i < numberOfAnswers && databaseFile.good(); i++){
		std::getline(databaseFile, temp);
		newQuestion.addAnswer(temp);
	}
	
	// Load the correct answer
	if(i == numberOfAnswers){
		std::getline(databaseFile, temp);
		for(int j = 0; j < temp.size(); j++){
			if(temp[j] > 96 && temp[j] < 105){
				index = (int)temp[j] - 97;
				newQuestion.setCorrectAnswerIndex(index);
			}
		}
		questions.push_back(newQuestion);
		numberOfQuestions++;
	}
};

void displayQuestions(int &correctQuestions, int &allQuestionsAsked, std::vector<Question> &questions, 
						const bool &seeCorrect, const int &numberOfQuestions){
	//system("cls");	// Linux version
	system("clear");	// Windows version

	std::string answer;
	std::cout << "\n\tYour actual score: " << (double)((double)correctQuestions / 
			(double)(allQuestionsAsked == 0 ? 1 : allQuestionsAsked)) * 100.0f << 
			"% (" << correctQuestions << "/" << allQuestionsAsked << ")\n\n";

	// Sorting Mechanism for Questions
	if (!(correctQuestions % numberOfQuestions)) std::random_shuffle(std::begin(questions), std::end(questions));
		else sort(questions.begin(), questions.end(), sortQuestions);

	std::cout << "\n\t" << questions[QUESTIONINDEX].getQuestion();
	allQuestionsAsked++;
	if(seeCorrect) std::cout << " [" << questions[QUESTIONINDEX].getNumberCorrect() << " correct numbers]";
	int numberOfAnswers = questions[QUESTIONINDEX].getAnswers().size();
	
	// Shuffle answers and display
	questions[QUESTIONINDEX].reorganizeAnswers();
	for(int i = 0; i < numberOfAnswers; i++) std::cout << "\n\t\t" << (char)(i + 'a') << ". " << 
												questions[QUESTIONINDEX].getAnswers()[i].desc;
	
	std::cout << "\n\n\tYour answer: ";
	std::getline(std::cin, answer);
	bool answeredCorrectly = false;

	// Compare user answer with the correct answer
	for(int i = 0; i < answer.size(); i++){
		if(answer[i] > 96 && answer[i] < 105){
			int answerIndex = (int)answer[i] - 97;
			// If index is out of scope
			if(answerIndex >= questions[QUESTIONINDEX].getAnswers().size()) invokeError(2);
			// If this part of answer is correct we continue checking
			else if(questions[QUESTIONINDEX].getAnswers()[answerIndex].isCorrect){
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

	// Output if the user was right or not
	if(!answeredCorrectly)
		displayCorrectAnswers(QUESTIONINDEX, questions);
	else {
		std::cout << "\n\t\t[CORRECT] Well done!";
		questions[QUESTIONINDEX].addAnsweredCorrectly();
		correctQuestions++;
	}

	std::cout <<"\n\n\t[NEXT QUESTION] Press ENTER";
	std::getline(std::cin, answer);
};

void displayCorrectAnswers(const int index, std::vector<Question> &questions){
	std::cout << "\n\t[WRONG] Correct answers:";
	for(int j = 0; j < questions[index].getAnswers().size(); j++)
		if(questions[index].getAnswers()[j].isCorrect)
			std::cout << "\n\t\t" << questions[index].getAnswers()[j].desc;
};

void checkQuestions(std::vector<Question> &questions) {
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
};