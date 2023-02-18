
struct stBaseMsg
{
	int nID;
	stBaseMsg()
	{
		nID = 0;
	}
};

const int Msg_UserInfo = 1000;
struct stUserInfo :public stBaseMsg
{
	char szUsername[50];
	char szPassword[50];
	stUserInfo()
	{
		memset(szUsername, 0, sizeof(szUsername));
		memset(szPassword, 0, sizeof(szPassword));
		nID = Msg_UserInfo;
	}
};

const int Msg_Chat = 1001;
struct stChat :public stBaseMsg
{
	char szContent[200];
	stChat()
	{
		memset(szContent, 0, sizeof(szContent));
		nID = Msg_Chat;
	}
	void Clear()
	{
		memset(szContent, 0, sizeof(szContent));
	}
};