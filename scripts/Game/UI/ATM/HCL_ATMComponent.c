class HCL_ATMComponentClass: ScriptComponentClass {}

class HCL_ATMComponent: ScriptComponent
{
    [Attribute(category: "ATM")]
    protected string m_MenuName;
    
    [Attribute(category: "ATM")]
    protected string m_UserName;
    
    [Attribute(category: "ATM")]
    protected int m_Balance;
    
    string GetName()
    {
        return m_MenuName;
    }
    
    string GetUserName()
    {
        return m_UserName;
    }
    
    int GetBalance()
    {
        return m_Balance;
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