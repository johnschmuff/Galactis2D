class CANNON: public WEAPON
{
public:
	void declareVar();
};

void CANNON::declareVar()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	alive = false;
}