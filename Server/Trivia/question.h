#pragma once
#include <vector>
#include <string>

class Question
{
public:
	Question();
	Question(int id, std::string question, std::vector<std::string> possibleAnswers, unsigned int correctAnswer);

	std::string getQuestion();
	std::vector<std::string> getPossibleAnswers();
	unsigned int getCorrectAnswer();
	int getQuestionId();

private:
	int m_id;
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	unsigned int m_correctAnswer;
};

