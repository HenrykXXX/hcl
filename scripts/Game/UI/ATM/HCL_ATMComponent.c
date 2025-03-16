class HCL_ATMComponentClass: ScriptComponentClass {}

class HCL_ATMComponent: ScriptComponent
{
    [Attribute(category: "ATM")]
    protected string m_MenuName;
    
    [Attribute(category: "ATM")]
    protected string m_UserName;
    
    [Attribute(category: "ATM", desc: "Player's bank account balance")]
    protected int m_AccountBalance;
    
    [Attribute(category: "ATM", desc: "Player's current cash on hand")]
    protected int m_PlayerBalance;
    
    string GetName()
    {
        return m_MenuName;
    }
    
    string GetUserName()
    {
        return m_UserName;
    }
    
    int GetAccountBalance()
    {
        return m_AccountBalance;
    }
    
    int GetPlayerBalance()
    {
        return m_PlayerBalance;
    }
    
    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);
        SetEventMask(owner, EntityEvent.INIT);
        owner.SetFlags(EntityFlags.ACTIVE, true);
        
        RplComponent rpl = RplComponent.Cast(owner.FindComponent(RplComponent));
        if (rpl) 
        {
            rpl.InsertToReplication();
        }
    }
} 