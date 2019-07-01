#include "question.h"

Question::Question()
	: m_id(-1), m_question(""), m_correctAnswer(0)
{
}

Question::Question(int id, std::string question, std::vector<std::string> possibleAnswers, unsigned int correctAnswer)
	: m_id(id), m_question(question), m_possibleAnswers(possibleAnswers), m_correctAnswer(correctAnswer)
{
}

std::string Question::getQuestion()
{
	return m_question;
}

std::vector<std::string> Question::getPossibleAnswers()
{
	return m_possibleAnswers;
}

unsigned int Question::getCorrectAnswer()
{
	return m_correctAnswer;
}

int Question::getQuestionId()
{
	return m_id;
}

bool Question::operator==(const Question & other) const
{
	return other.m_id == m_id;
}
