class HCL_SpawnMenuComponentClass: ScriptComponentClass {}

class HCL_SpawnMenuComponent: ScriptComponent
{
    [Attribute(category: "Spawn Menu")]
    protected string m_MenuName;
    
    string GetName()
    {
        return m_MenuName;
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