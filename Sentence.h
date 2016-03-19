#pragma once
#include "SentenceItem.h"
#include "String.h"
#include <deque>
#include <string>

class Sentence : public SentenceItem
{
public:
	Sentence(const std::string& name)
		: name(name)
	{
	}

	void WriteXml(std::ostream& out) const final
	{
		out << "<" << name << ">";
		for (auto& item : nodes)
			item->WriteXml(out);
		out << "</" << name << ">";
	}

	Position GetPosition() const final
	{
		if (nodes.empty())
			return{ "", 0, 0 };
		return nodes.front()->GetPosition();
	}

	const std::string& GetName() const
	{
		return name;
	}
	const std::deque<SentenceItemPtr>& GetNodes() const
	{
		return nodes;
	}
	std::deque<SentenceItemPtr>& GetNodes()
	{
		return nodes;
	}

	void AddNode(SentenceItemPtr node)
	{
		nodes.push_back(node);
	}
	void AddNodeToFront(SentenceItemPtr node)
	{
		nodes.push_front(node);
	}

	bool CanBeReplacedWithChildren(const std::string& parentName) const final
	{
		return String::EndsWith(name, "-opt") ||
			String::EndsWith(name, "*") ||
			String::EndsWith(name, "'") ||
			(String::EndsWith(name, "-list") && parentName == name);
	}

	std::deque<SentenceItemPtr> GetChildren() const final
	{
		return nodes;
	}

	void Simplify() final
	{
		for (auto index = 0u; index < nodes.size(); )
		{
			auto item = nodes[index];
			if (item->CanBeReplacedWithChildren(name))
			{
				nodes.erase(nodes.begin() + index);
				auto children = item->GetChildren();
				nodes.insert(nodes.begin() + index, children.begin(), children.end());
			}
			else
			{
				item->Simplify();
				++index;
			}
		}
	}

private:
	std::string name;
	std::deque<SentenceItemPtr> nodes;
};

#if 0
        /// <summary>
        /// This function is used to access an element of the sentence.  The index to begin searching
        /// is passed in as well as a flag to indicate whether to keep scanning forward.  An optional
        /// name for matching (name of sentence to find) and an optional name of the sentence to stop
        /// searching at are also provided.  When blank these will be ignored.  If the MustExist flag
        /// is specified but the sentence isn't found, then an exception is thrown instead of returning
        /// null.  The item at the given position must be a sentence and not a leaf token node.
        /// </summary>
        /// <param name="Index">Index to begin searching.  On output is the matching item's index.</param>
        /// <param name="ScanForward">Flag to allow searching past original index.</param>
        /// <param name="MustExist">Flag to indicate whether item must exist.</param>
        /// <param name="OptName">Optional filter for the name of the sentence to find.</param>
        /// <param name="OptStopAtSentence">Flag indicating if we should stop searching at a
        /// particular sentence (true) or a token (false).</param>
        /// <param name="OptStopAtName">Name of the particular sentence or token to stop at.  When
        /// blank the search will not stop until the end of the sentence.</param>
        /// <returns>Sentence that was found or null.</returns>
        public Sentence GetSentence(ref int Index, bool ScanForward, bool MustExist, string OptName, bool OptStopAtSentence, string OptStopAtName)
        {
            for (; Index < mNodes.Count; ++Index)
            {
                Sentence child = mNodes[Index] as Sentence;
                if (child != null && (string.IsNullOrEmpty(OptName) || child.Name == OptName))
                    return child;
                if (!ScanForward ||
                    OptStopAtSentence && child != null && !string.IsNullOrEmpty(OptStopAtName) && OptStopAtName == child.Name ||
                    !OptStopAtSentence && child == null && !string.IsNullOrEmpty(OptStopAtName) && OptStopAtName == (mNodes[Index] as SentenceToken).Token.Value)
                    break;
            }
            if (MustExist)
            {
                Position pos = Position;
                throw new InvalidOperationException(string.Format("Missing child sentence from sentence; grammar error?\nParent: {0}, Child: {1}\n{2}", mName, OptName, Position == null ? "Empty production" : Position.ToString()));
            }
            return null;
        }

        #region GetSentence overloads.
        public Sentence GetSentence(ref int Index, bool ScanForward, bool MustExist, string OptName)
        {
            return GetSentence(ref Index, ScanForward, MustExist, OptName, false, string.Empty);
        }
        public Sentence GetSentence(ref int Index, bool ScanForward, bool MustExist)
        {
            return GetSentence(ref Index, ScanForward, MustExist, string.Empty);
        }
        public Sentence GetSentence(int Index, string Name)
        {
            return GetSentence(ref Index, false, true, Name);
        }
        public Sentence GetSentence(int Index)
        {
            return GetSentence(Index, string.Empty);
        }

        /// <summary>
        /// This function will get the sentence at the given index with the given name
        /// if it exists, or return null otherwise.  Useful for checking for optional
        /// production items.
        /// </summary>
        /// <param name="Index">Index to search at.</param>
        /// <param name="Name">Name of setnence.</param>
        /// <returns>Sentence with given name at index or null.</returns>
        public Sentence GetSentenceOpt(int Index, string Name)
        {
            return GetSentence(ref Index, false, false, Name);
        }
        #endregion

        /// <summary>
        /// This function will get the token at the given location.  If the
        /// index is out of bound or the item at that index is not a token
        /// then an exception is thrown.
        /// </summary>
        /// <param name="Index">Index of the item in the sentence.</param>
        /// <returns>Token at given index.</returns>
        public Token GetToken(int Index)
        {
            if (Index >= mNodes.Count)
                throw new InvalidOperationException("Index out of bounds.");
            SentenceToken leaf = mNodes[Index] as SentenceToken;
            if (leaf == null)
                throw new InvalidOperationException("Item at index was not a leaf.");
            return leaf.Token;
        }

        /// <summary>
        /// This function will get the token at the given index so long
        /// as the index is in bound as the item at that index is a token.
        /// Otherwise the function will just return null (not throw).
        /// </summary>
        /// <param name="Index">Index of the item in the sentence.</param>
        /// <returns>Token at given index or null.</returns>
        public Token GetTokenOpt(int Index)
        {
            if (Index >= mNodes.Count)
                return null;
            SentenceToken leaf = mNodes[Index] as SentenceToken;
            return leaf == null ? null : leaf.Token;
        }
    }
}
#endif
