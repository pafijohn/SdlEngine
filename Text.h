#include <SDL.h>
#include <string>
#include <vector>

#include "Renderable.h"
#include "Positionable.h"

struct TextChunk
{
	std::string text;
	std::string identifier;
};

class Text: public Renderable, public Positionable
{
	std::vector< TextChunk* > chunks;
	int background;
	
public:
	enum
	{
		WHITE,
		BEIGE,
		GREY,
		BROWN,
		NONE
	};
	
	Text();
	~Text();
	
	void Clear();
	void SetBackground( int background );
	
	virtual bool Update();
	virtual void Render();
	
	void AddText( const std::string& text, const std::string& identifier = "" );
};
