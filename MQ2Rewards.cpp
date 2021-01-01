#include "../MQ2Plugin.h"

PreSetup("MQ2Rewards");
PLUGIN_VERSION(1.0);

const int RewardChoiceColumn = 0;

struct RewardItem
{
	bool exists;
	CPageWnd* pagePtr;
	char szName[MAX_STRING];
};

struct RewardOption
{
	CListWnd* listWndPtr;
	int index;
	char szName[MAX_STRING];
};

struct RewardItem Rewards[20];
struct RewardOption RewardOptions[20];
struct RewardOption RewardOptionItems[20];

static inline bool IsNumeric(PCHAR String)
{
	if (*String == 0)
		return false;
	if (*String == '-')
		String++;
	while (*String)
	{
		if (!((*String >= '0' && *String <= '9') || *String == '.'))
			return false;
		++String;
	}
	return true;
}

CTabWnd* GetTabWindow()
{
	CXWnd* rewardWnd = (CXWnd*)FindMQ2Window("RewardSelectionWnd");
	if (!rewardWnd)
	{
		return nullptr;
	}
	CTabWnd* tabWindow = (CTabWnd*)rewardWnd->GetFirstChildWnd();
	return tabWindow;
}

CPageWnd* GetSelectedRewardPage()
{
	CTabWnd* tabWindow = GetTabWindow();
	if (!tabWindow)
	{
		return nullptr;
	}

	CPageWnd* pageWindow = tabWindow->GetCurrentPage();
	return pageWindow;
}

CListWnd* GetOptionListControl(CPageWnd* pageWindow)
{
	return (CListWnd*)((CSidlScreenWnd*)(pageWindow))->GetChildItem("RewardSelectionOptionList");
}

CListWnd* GetOptionItemListControl(CPageWnd* pageWindow)
{
	return (CListWnd*)((CSidlScreenWnd*)(pageWindow))->GetChildItem("RewardSelectionItemList");
}

CPageWnd* GetPagePtr(CListWnd* listPtr)
{
	return (CPageWnd*)listPtr->GetParentWindow();
}

CPageWnd* GetCurrentPage()
{
	CTabWnd* tabWindow = GetTabWindow();
	if (!tabWindow) {
		return nullptr;
	}

	return tabWindow->GetCurrentPage();
}

int GetRewardsCount()
{
	CTabWnd* tabWindow = GetTabWindow();
	if (!tabWindow)
	{
		return -1;
	}

	int rewardCount = -1;

	_CSIDLWND* pageWindow = tabWindow->GetFirstChildWnd();
	while (pageWindow)
	{
		rewardCount++;

		pageWindow = pageWindow->GetNextSiblingWnd();
	}

	return rewardCount;
}

int GetRewardOptionCount(RewardItem* rewardPtr)
{
	CListWnd* listWndPtr = GetOptionListControl(rewardPtr->pagePtr);
	if (!listWndPtr) {
		return -1;
	}

	return listWndPtr->ItemsArray.Count;
}

//char* GetRewardOptionText(CListWnd* listWindow, int index = -1)
//{
//	if (index < 0) {
//		index = listWindow->GetCurSel();
//	}
//
//	if (index < 0) {
//		return nullptr;
//	}
//
//	CXStr Str;
//	CHAR szOut[MAX_STRING] = { 0 };
//	listWindow->GetItemText(&Str, index, RewardChoiceColumn);
//	GetCXStr(Str.Ptr, szOut, MAX_STRING);
//	return szOut;
//}

int GetRewardOptionItemCount(CPageWnd* pagePtr)
{
	CListWnd* listWndPtr = GetOptionItemListControl(pagePtr);
	if (!listWndPtr) {
		return -1;
	}

	return listWndPtr->ItemsArray.Count;
}

RewardItem FindRewardInternal(char* szReward)
{
	RewardItem rewardItem;
	rewardItem.exists = false;

	if (!szReward[0]) {
		return rewardItem;
	}

	bool isNumber = IsNumeric(szReward);

	if (isNumber) {
		int rewardNumber = atoi(szReward) - 1;
		if (rewardNumber < 0 || rewardNumber > 9)
		{
			return rewardItem;
		}

		CTabWnd* tabWindow = GetTabWindow();
		rewardItem.pagePtr = tabWindow->GetPageFromTabIndex(rewardNumber);
		if (!rewardItem.pagePtr)
		{
			return rewardItem;
		}

		rewardItem.exists = true;
		return rewardItem;
	}

	CTabWnd* tabWindow = GetTabWindow();
	CPageWnd* pageWindow = (CPageWnd*)tabWindow->GetFirstChildWnd();
	while (pageWindow)
	{
		if (pageWindow->TabText && pageWindow->TabText->Text && !_stricmp(szReward, pageWindow->TabText->Text)) {
			rewardItem.exists = true;
			rewardItem.pagePtr = pageWindow;
			return rewardItem;
		}

		pageWindow = (CPageWnd*)pageWindow->GetNextSiblingWnd();
	}

	return rewardItem;
}

RewardOption FindListItemInternal(CListWnd* listPtr, char* szName)
{
	RewardOption itemSelection;
	itemSelection.index = -1;

	if (!szName[0]) {
		return itemSelection;
	}

	if (!listPtr || !listPtr->ItemsArray.Count) {
		return itemSelection;
	}

	bool isNumber = IsNumeric(szName);

	if (isNumber) {
		int itemNumber = atoi(szName) - 1;
		if (itemNumber < 0 || itemNumber > listPtr->ItemsArray.Count)
		{
			return itemSelection;
		}

		itemSelection.index = itemNumber;
		itemSelection.listWndPtr = listPtr;

		CXStr itemText;
		CHAR szOut[MAX_STRING] = { 0 };
		itemSelection.listWndPtr->GetItemText(&itemText, itemNumber, 0);
		GetCXStr(itemText.Ptr, itemSelection.szName, MAX_STRING);

		return itemSelection;
	}

	CXStr itemText;
	CHAR szOut[MAX_STRING] = { 0 };
	int optionNumber = -1;
	for (int index = 0; index < listPtr->ItemsArray.Count; index++)
	{
		listPtr->GetItemText(&itemText, index, 0);
		GetCXStr(itemText.Ptr, szOut, MAX_STRING);

		if (!_stricmp(szOut, szName)) {
			itemSelection.index = index;
			itemSelection.listWndPtr = listPtr;
			strcpy_s(itemSelection.szName, szOut);
			return itemSelection;
		}
	}

	return itemSelection;
}

RewardOption FindRewardOption(CPageWnd* pagePtr, char* szName)
{
	CListWnd* listPtr = GetOptionListControl(pagePtr);
	return FindListItemInternal(listPtr, szName);
}

RewardOption FindRewardOptionItem(CPageWnd* pagePtr, char* szName)
{
	CListWnd* listPtr = GetOptionItemListControl(pagePtr);
	return FindListItemInternal(listPtr, szName);
}

BOOL SelectReward(RewardItem* rewardPtr)
{
	CTabWnd* tabWindow = GetTabWindow();
	if (!tabWindow) {
		return FALSE;
	}

	for (int index = 0; index <= tabWindow->PageArray.Count; index++)
	{
		CPageWnd* pageWindowCheck = tabWindow->GetPageFromTabIndex(index);
		if (!pageWindowCheck || !pageWindowCheck->TabText)
		{
			continue;
		}

		if (!_stricmp(pageWindowCheck->TabText->Text, rewardPtr->pagePtr->TabText->Text))
		{
			tabWindow->SetPage(index);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL ClaimReward(RewardItem* rewardPtr)
{
	CListWnd* pList = GetOptionListControl(rewardPtr->pagePtr);
	int selectedOption = pList->GetCurSel();
	if (selectedOption < 0 && pList->ItemsArray.Count)
	{
		return FALSE;
	}

	CSidlScreenWnd* selectOptionButton = (CSidlScreenWnd*)rewardPtr->pagePtr->GetChildItem("RewardSelectionChooseButton");
	if (!selectOptionButton || !selectOptionButton->IsEnabled())
	{
		return FALSE;
	}

	WriteChatf("[MQ2Rewards] Claiming reward '%s', option %d.", rewardPtr->pagePtr->TabText->Text, selectedOption);
	// WriteChatf("[MQ2Rewards] Claiming reward '%s', option '%s'.", pageWindow->TabText->Text, GetRewardOptionText(pList, selectedOption));
	SendWndClick2((CXWnd*)selectOptionButton, "leftmouseup");

	return TRUE;
}
 
VOID SelectListOption(CListWnd* pListWindow, int index)
{
	pListWindow->SetCurSel(index);

	CXRect listrect = pListWindow->GetItemRect(index, 0);
	CXPoint listpt = listrect.CenterPoint();
	((CXWnd*)pListWindow)->HandleLButtonDown(&listpt, 0);
	((CXWnd*)pListWindow)->HandleLButtonUp(&listpt, 0);
	WeDidStuff();
}

VOID CommandDisplayHelp()
{
	WriteChatColor("MQ2Rewards", USERCOLOR_WHO);
	WriteChatColor("", USERCOLOR_WHO);
	WriteChatColor("usage: /rewards [param] (arg1)", USERCOLOR_WHO);
	WriteChatColor("   params:", USERCOLOR_WHO);
	WriteChatColor("   [reward: (reward)	Selects the specified reward (tab) name/# if found but does not claim]", USERCOLOR_WHO);
	WriteChatColor("   [option: (option)    For the current reward, selects the specified reward name/# if found but does not claim]", USERCOLOR_WHO);
	WriteChatColor("   [                    NOTE: Known issue; option changes, but option reward list does not properly reflect.]", USERCOLOR_WHO);
	WriteChatColor("   [claim:  Chooses the currently selected reward/option.]", USERCOLOR_WHO);
}

VOID CommandSelectOption(PSPAWNINFO pChar, PCHAR szLine)
{
	char szOption[MAX_STRING] = { 0 };
	GetArg(szOption, szLine, 2);

	// Now see if we were asked to select a reward as well
	if (!szOption[0])
	{
		CommandDisplayHelp();
		return;
	}

	CPageWnd* pageWindow = GetCurrentPage();
	if (!pageWindow)
	{
		return;
	}

	bool isNumber = IsNumeric(szOption);
	int optionNumber = atoi(szOption) - 1;

	CListWnd* pList = GetOptionListControl(pageWindow);
	if (!pList)
	{
		return;
	}

	int itemCount = pList->ItemsArray.Count;

	if (isNumber) {
		if (optionNumber > itemCount) {
			// TODO: Indicate max number of options
			MacroError("Option specified is out of bounds.  Must be > 1 and < # of options in list: '%d'", optionNumber);
			return;
		}

		SelectListOption(pList, optionNumber);
	}
	else {
		CXStr itemText;
		CHAR szOut[MAX_STRING] = { 0 };
		optionNumber = -1;
		for (int index = 0; index < itemCount; index++)
		{
			pList->GetItemText(&itemText, index, 0);
			GetCXStr(itemText.Ptr, szOut, MAX_STRING);

			if (!_stricmp(szOut, szOption)) {
				optionNumber = index;
				break;
			}
		}

		if (optionNumber >= 0) {
			pList->SetCurSel(optionNumber);
		}
		else {
			MacroError("Specified option does not exist: '%s'", szOption);
			return;
		}
	}
}

VOID CommandSelectReward(PSPAWNINFO pChar, PCHAR szLine)
{
	char szReward[MAX_STRING] = { 0 };
	GetArg(szReward, szLine, 2);

	RewardItem reward = FindRewardInternal(szReward);
	if (!reward.exists)
	{
		MacroError("Reward not found: '%s'", szReward);
		return;
	}

	if (!SelectReward(&reward))
	{
		MacroError("Reward unable to be selected: '%s'", szReward);
		return;
	}
}

VOID CommandClaimReward(PSPAWNINFO pChar, PCHAR szLine)
{
	CPageWnd* pageWindow = GetSelectedRewardPage();
	if (!pageWindow)
	{
		MacroError("No reward has been selected.");
		return;
	}

	// If no option was selected and there ARE options to select, boom.
	CListWnd* pList = GetOptionListControl(pageWindow);
	int selectedOption = pList->GetCurSel();
	if (selectedOption < 0 && pList->ItemsArray.Count)
	{
		MacroError("No option has been selected to claim.");
		return;
	}

	CSidlScreenWnd* selectOptionButton = (CSidlScreenWnd*)pageWindow->GetChildItem("RewardSelectionChooseButton");
	if (!selectOptionButton || !selectOptionButton->IsEnabled())
	{
		MacroError("No option has been selected to claim.");
		return;
	}

	WriteChatf("[MQ2Rewards] Claiming reward '%s', option %d.", pageWindow->TabText->Text, selectedOption);
	// WriteChatf("[MQ2Rewards] Claiming reward '%s', option '%s'.", pageWindow->TabText->Text, GetRewardOptionText(pList, selectedOption));
	SendWndClick2((CXWnd*)selectOptionButton, "leftmouseup");
}

VOID RewardsCommand(PSPAWNINFO pChar, PCHAR szLine)
{
	char arg1[MAX_STRING] = { 0 };
	GetArg(arg1, szLine, 1);
	CHAR szTemp[32] = { 0 };

	if (!_stricmp(arg1, "help"))
	{
		CommandDisplayHelp();
		return;
	}

	if (!_stricmp(arg1, "reward"))
	{
		CommandSelectReward(pChar, szLine);
		return;
	}

	if (!_stricmp(arg1, "option"))
	{
		CommandSelectOption(pChar, szLine);
		return;
	}

	if (!_stricmp(arg1, "claim"))
	{
		CommandClaimReward(pChar, szLine);
		return;
	}

	CommandDisplayHelp();
}


class MQ2RewardsType* pRewardsType = nullptr;
class MQ2RewardType* pRewardType = nullptr;
class MQ2RewardOptionType* pRewardOptionType = nullptr;
class MQ2RewardOptionItemType* pRewardOptionItemType = nullptr;

class MQ2RewardOptionItemType : public MQ2Type {
public:
	enum Members {
		Text
	};

	MQ2RewardOptionItemType() : MQ2Type("RewardOptionItem")
	{
		TypeMember(Text);
	}

	~MQ2RewardOptionItemType() {}

	bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR& Dest)
	{
		PMQ2TYPEMEMBER pMember = MQ2RewardOptionItemType::FindMember(Member);
		if (!pMember)
			return false;
		if (!pLocalPlayer)
			return false;

		if (!VarPtr.Ptr)
			return false;

		RewardOption* item = (RewardOption*)VarPtr.Ptr;
		if (!item) {
			return false;
		}

		PCHARINFO pCharInfo = GetCharInfo();
		switch ((Members)pMember->ID) {
		case Text:
			if (!item->szName)
				return false;
			strcpy_s(DataTypeTemp, item->szName);
			Dest.Ptr = &DataTypeTemp[0];
			Dest.Type = pStringType;
			return true;
		}
		return false;
	}
	bool ToString(MQ2VARPTR VarPtr, PCHAR Destination)
	{
		if (!VarPtr.Ptr) {
			return false;
		}
		RewardOption* itemPtr = (RewardOption*)VarPtr.Ptr;
		if (!itemPtr) {
			return false;
		}
		strcpy_s(Destination, MAX_STRING, itemPtr->szName);
		return true;
	}
	void InitVariable(MQ2VARPTR& VarPtr) {
		VarPtr.Ptr = malloc(sizeof(RewardOption));
		VarPtr.HighPart = 0;
		ZeroMemory(VarPtr.Ptr, sizeof(RewardOption));
	}
	void FreeVariable(MQ2VARPTR& VarPtr) {
		free(VarPtr.Ptr);
	}
	bool FromData(MQ2VARPTR& VarPtr, MQ2TYPEVAR& Source)
	{
		if (Source.Type != pRewardOptionItemType)
		{
			return false;
		}

		memcpy(VarPtr.Ptr, Source.Ptr, sizeof(RewardOption));

		return true;
	}
	bool FromString(MQ2VARPTR& VarPtr, PCHAR Source)
	{
		return false;
	}
};

class MQ2RewardOptionType : public MQ2Type {
public:
	enum Members {
		Text,
		Selected,
		Select,
		ItemCount,
		Item
	};

	MQ2RewardOptionType() : MQ2Type("RewardOption")
	{
		TypeMember(Text);
		TypeMember(Selected);
		TypeMember(Select);
		TypeMember(ItemCount);
		TypeMember(Item);
	}

	~MQ2RewardOptionType() {}

	bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR& Dest)
	{
		PMQ2TYPEMEMBER pMember = MQ2RewardOptionType::FindMember(Member);
		if (!pMember)
			return false;
		if (!pLocalPlayer)
			return false;

		if (!VarPtr.Ptr)
			return false;

		RewardOption* item = (RewardOption*)VarPtr.Ptr;
		if (!item) {
			return false;
		}

		PCHARINFO pCharInfo = GetCharInfo();
		switch ((Members)pMember->ID) {
		case Text:
			if (!item->szName)
				return false;
			strcpy_s(DataTypeTemp, item->szName);
			Dest.Ptr = &DataTypeTemp[0];
			Dest.Type = pStringType;
			return true;
		case Selected:
			if (!item->listWndPtr)
				return false;
			Dest.Int = item->listWndPtr->GetCurSel() == item->index;
			Dest.Type = pBoolType;
			return true;
		case Select:
			item->listWndPtr->SetCurSel(item->index);
			Dest.Int = 1;
			Dest.Type = pBoolType;
			return true;
		case ItemCount:
			Dest.Int = GetRewardOptionItemCount(GetPagePtr(item->listWndPtr));
			Dest.Type = pIntType;
			return Dest.Int >= 0;
		case Item:
			if (Index[0]) {
				RewardOption option = FindRewardOptionItem(GetPagePtr(item->listWndPtr), Index);
				if (option.index > -1)
				{
					memcpy(&RewardOptionItems[0], &option, sizeof(RewardOption));
					Dest.Ptr = &RewardOptions[0];
					Dest.Type = pRewardOptionItemType;
					return true;
				}
			}
		}
		return false;
	}

	bool ToString(MQ2VARPTR VarPtr, PCHAR Destination)
	{
		if (!VarPtr.Ptr) {
			return false;
		}
		RewardOption* itemPtr = (RewardOption*)VarPtr.Ptr;
		if (!itemPtr) {
			return false;
		}
		strcpy_s(Destination, MAX_STRING, itemPtr->szName);
		return true;
	}

	void InitVariable(MQ2VARPTR& VarPtr) {
		VarPtr.Ptr = malloc(sizeof(RewardOption));
		VarPtr.HighPart = 0;
		ZeroMemory(VarPtr.Ptr, sizeof(RewardOption));
	}

	void FreeVariable(MQ2VARPTR& VarPtr) {
		free(VarPtr.Ptr);
	}

	bool FromData(MQ2VARPTR& VarPtr, MQ2TYPEVAR& Source)
	{
		if (Source.Type != pRewardOptionType)
		{
			return false;
		}

		memcpy(VarPtr.Ptr, Source.Ptr, sizeof(RewardOption));

		return true;
	}

	bool FromString(MQ2VARPTR& VarPtr, PCHAR Source)
	{
		return false;
	}
};

class MQ2RewardType : public MQ2Type {
public:
	enum Members {
		Text,
		Selected,
		Select,
		Claim,
		Options,
		Option,
		Items,
		Item,
	};

	MQ2RewardType() :MQ2Type("RewardItem")
	{
		TypeMember(Text);
		TypeMember(Selected);
		TypeMember(Select);
		TypeMember(Claim);
		TypeMember(Options);
		TypeMember(Option);
		TypeMember(Items);
		TypeMember(Item);
	}
	~MQ2RewardType() {}

	bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR& Dest)
	{
		PMQ2TYPEMEMBER pMember = MQ2RewardType::FindMember(Member);
		if (!pMember)
			return false;
		if (!pLocalPlayer)
			return false;

		if (!VarPtr.Ptr)
			return false;
		
		RewardItem* item = (RewardItem*)VarPtr.Ptr;
		if (!item || !item->pagePtr) {
			return false;
		}

		PCHARINFO pCharInfo = GetCharInfo();
		switch ((Members)pMember->ID) {
		case Text:
			strcpy_s(DataTypeTemp, item->pagePtr->TabText->Text);
			Dest.Ptr = &DataTypeTemp[0];
			Dest.Type = pStringType;
			return true;
		case Selected:
			Dest.Int = item->pagePtr->IsVisible();
			Dest.Type = pBoolType;
			return true;
		case Select:
			Dest.Int = SelectReward(item);
			Dest.Type = pBoolType;
			return true;
		case Claim:
			Dest.Int = ClaimReward(item);
			Dest.Type = pBoolType;
			return Dest.Int == TRUE;
		case Options:
			Dest.Int = GetRewardOptionCount(item);
			Dest.Type = pIntType;
			return true;
		case Option:
			if (Index[0]) {
				RewardOption option = FindRewardOption(item->pagePtr, Index);
				if (option.index > -1)
				{
					memcpy(&RewardOptions[0], &option, sizeof(RewardOption));
					Dest.Ptr = &RewardOptions[0];
					Dest.Type = pRewardOptionType;
					return true;
				}
			}
			return false;
		case Items:
			// TODO: Track whether a page has been visited/loaded. Only force it once.
			SelectReward(item);
			Dest.Int = GetRewardOptionItemCount(item->pagePtr);
			Dest.Type = pIntType;
			return true;
		case Item:
			if (Index[0]) {
				// TODO: Track whether a page has been visited/loaded. Only force it once.
				SelectReward(item);
				RewardOption option = FindRewardOptionItem(item->pagePtr, Index);
				if (option.index > -1)
				{
					memcpy(&RewardOptionItems[0], &option, sizeof(RewardOption));
					Dest.Ptr = &RewardOptionItems[0];
					Dest.Type = pRewardOptionItemType;
					return true;
				}
			}
			break;
		}
		return false;
	}
	bool ToString(MQ2VARPTR VarPtr, PCHAR Destination)
	{
		if (!VarPtr.Ptr) {
			return false;
		}
		RewardItem* item = (RewardItem*)VarPtr.Ptr;
		if (!item) {
			return false;
		}
		strcpy_s(Destination, MAX_STRING, item->pagePtr->TabText->Text);
		return true;
	}
	void InitVariable(MQ2VARPTR& VarPtr) {
		VarPtr.Ptr = malloc(sizeof(RewardItem));
		VarPtr.HighPart = 0;
		ZeroMemory(VarPtr.Ptr, sizeof(RewardItem));
	}
	void FreeVariable(MQ2VARPTR& VarPtr) {
		free(VarPtr.Ptr);
	}
	bool FromData(MQ2VARPTR& VarPtr, MQ2TYPEVAR& Source)
	{
		if (Source.Type != pRewardType)
		{
			return false;
		}

		memcpy(VarPtr.Ptr, Source.Ptr, sizeof(RewardItem));

		return true;
	}
	bool FromString(MQ2VARPTR& VarPtr, PCHAR Source)
	{
		return false;
	}
};

class MQ2RewardsType : public MQ2Type {
public:
	enum Members {
		Reward,
		Count,
	};

	MQ2RewardsType() :MQ2Type("Rewards")
	{
		TypeMember(Reward);
		TypeMember(Count);
	}
	~MQ2RewardsType() {}

	bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR& Dest)
	{
		PMQ2TYPEMEMBER pMember = MQ2RewardsType::FindMember(Member);
		if (!pMember)
			return false;
		if (!pLocalPlayer)
			return false;

		PCHARINFO pCharInfo = GetCharInfo();
		switch ((Members)pMember->ID) {
		case Reward:
			if (Index[0]) {
				RewardItem reward = FindRewardInternal(Index);
				if (reward.exists && reward.pagePtr) {
					memcpy(&Rewards[0], &reward, sizeof(RewardItem));
					Dest.Ptr = &Rewards[0];
					Dest.Type = pRewardType;
					return true;
				}
			}
			break;
		case Count:
			Dest.Int = GetRewardsCount();
			Dest.Type = pIntType;
			return true;
		}
		return false;
	}
	bool FromData(MQ2VARPTR& VarPtr, MQ2TYPEVAR& Source)
	{
		return false;
	}
	bool FromString(MQ2VARPTR& VarPtr, PCHAR Source)
	{
		return false;
	}
};

BOOL TloRewards(char* szIndex, MQ2TYPEVAR& Dest)
{
	Dest.DWord = 1;
	Dest.Type = pRewardsType;
	return true;
}

// Called once, when the plugin is to initialize
PLUGIN_API VOID InitializePlugin(VOID)
{
	AddCommand("/rewards", RewardsCommand);
	pRewardsType = new MQ2RewardsType;
	pRewardType = new MQ2RewardType;
	pRewardOptionType = new MQ2RewardOptionType;
	pRewardOptionItemType = new MQ2RewardOptionItemType;
	AddMQ2Data("Rewards", TloRewards);
}

PLUGIN_API VOID ShutdownPlugin(VOID)
{
	RemoveCommand("/rewards");
	RemoveMQ2Data("Rewards");
	delete pRewardsType;
	delete pRewardType;
	delete pRewardOptionType;
	delete pRewardOptionItemType;
}
