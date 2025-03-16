class HCL_SpawnMenuUI : ChimeraMenuBase
{
	protected HCL_SpawnMenuComponent m_SpawnMenu;
	protected Widget m_wRoot;
	protected TextWidget m_wHeaderText;
	protected ButtonWidget m_Button1;
	protected ButtonWidget m_Button2;
	
	void SetSpawnMenu(HCL_SpawnMenuComponent spawnMenu)
	{
		m_SpawnMenu = spawnMenu;
		
		if (m_wHeaderText && m_SpawnMenu)
		{
			m_wHeaderText.SetText(m_SpawnMenu.GetName());
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnMenuInit()
	{
		super.OnMenuInit();
		
		m_wRoot = GetRootWidget();
		m_wHeaderText = TextWidget.Cast(m_wRoot.FindAnyWidget("HeaderText"));
		
		// Get button references
		m_Button1 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_S1"));
		m_Button2 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_S2"));
	}
	
	//------------------------------------------------------------------------------------------------
	protected void TeleportPlayer(vector position)
	{
		if (position == vector.Zero)
			return;
			
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController)
			return;
			
		IEntity playerEntity = playerController.GetMainEntity();
		if (!playerEntity)
			return;
			
		// Set player position
		playerEntity.SetOrigin(position);
		
		// Close the menu after teleporting
		Close();
	}
	
	//------------------------------------------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (!m_SpawnMenu)
			return false;
			
		if (w == m_Button1)
		{
			TeleportPlayer(m_SpawnMenu.GetSpawnPosition1());
			return true;
		}
		else if (w == m_Button2)
		{
			TeleportPlayer(m_SpawnMenu.GetSpawnPosition2());
			return true;
		}
		
		return false;
	}
}