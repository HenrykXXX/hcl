[BaseContainerProps()]
class HCL_ATMAction : ScriptedUserAction
{
    protected HCL_ATMComponent m_ATMComponent;
    
    //------------------------------------------------------------------------------------------------
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent) 
    {
        m_ATMComponent = HCL_ATMComponent.Cast(pOwnerEntity.FindComponent(HCL_ATMComponent));
        if (!m_ATMComponent) 
        {
            Print("Error! Could not find ATM component for this action.", LogLevel.ERROR);
            return;
        }
    }
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {   
        if (!m_ATMComponent) return;
        
        SCR_PlayerController scrPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
        if (!scrPlayerController || pUserEntity != scrPlayerController.GetMainEntity()) 
            return;
        
        // Open the ATM menu
        MenuBase menuBase = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.HCL_ATMMenu);
        HCL_ATMMenuUI menu = HCL_ATMMenuUI.Cast(menuBase);
        if (menu)
            menu.SetATMComponent(m_ATMComponent);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBeShownScript(IEntity user)
    {
        if (!m_ATMComponent) 
            return false;
            
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {
        if (!m_ATMComponent) 
            return false;
        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "Use ATM";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool HasLocalEffectOnlyScript()
    {
        return true;
    }
} 