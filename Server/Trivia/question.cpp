#include "question.h"

Question::Question()
	: m_id(-1), m_question(""), m_correctAnswer(0)
{
}

Question::Question(int id, std::string question, std::vector<std::string> possibleAnswers, unsigned int correctAnswer)
	: m_id(id), m_question(question), m_possibleAnswers(possibleAnswers), m_correctAnswer(correctAnswer)
{
}

/**
 * Get the question
 *
 * @return The question
 */
std::string Question::getQuestion()
{
	return m_question;
}

/**
 * Get the possible answers for the question
 *
 * @return The possible answers
 */
std::vector<std::string> Question::getPossibleAnswers()
{
	return m_possibleAnswers;
}

/**
 * Get the correct answer
 *
 * @return The correct answer
 */
unsigned int Question::getCorrectAnswer()
{
	return m_correctAnswer;
}

/**
 * Get the question id
 *
 * @return The question id
 */
int Question::getQuestionId()
{
	return m_id;
}

/**
 * Compare 2 questions
 *
 * @param other The other question
 * @return Are the questions the same
 */
bool Question::operator==(const Question & other) const
{
	return other.m_id == m_id;
}
