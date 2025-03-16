[BaseContainerProps()]
class HCL_SpawnMenuAction : ScriptedUserAction
{
    protected HCL_SpawnMenuComponent m_SpawnMenu;
    
    //------------------------------------------------------------------------------------------------
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent) 
    {
        m_SpawnMenu = HCL_SpawnMenuComponent.Cast(pOwnerEntity.FindComponent(HCL_SpawnMenuComponent));
        if (!m_SpawnMenu) 
        {
            Print("Error! Could not find spawn menu component for this action.", LogLevel.ERROR);
            return;
        }
    }
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {   
        if (!m_SpawnMenu) return;
        
        SCR_PlayerController scrPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
        if (!scrPlayerController || pUserEntity != scrPlayerController.GetMainEntity()) 
            return;
        
        // Open the spawn menu
        MenuBase menuBase = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.HCL_SpawnMenu);
        HCL_SpawnMenuUI menu = HCL_SpawnMenuUI.Cast(menuBase);
        if (menu)
            menu.SetSpawnMenu(m_SpawnMenu);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBeShownScript(IEntity user)
    {
        if (!m_SpawnMenu) 
            return false;
            
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {
        if (!m_SpawnMenu) 
            return false;
        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "Open Spawn Menu";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool HasLocalEffectOnlyScript()
    {
        return true;
    }
} 