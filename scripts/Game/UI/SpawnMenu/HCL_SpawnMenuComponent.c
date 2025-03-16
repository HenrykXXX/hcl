class HCL_SpawnMenuComponentClass: ScriptComponentClass {}

class HCL_SpawnMenuComponent: ScriptComponent
{
    [Attribute(category: "Spawn Menu")]
    protected string m_MenuName;
    
    [Attribute(category: "Spawn Menu", desc: "First spawn position")]
    protected ref PointInfo m_SpawnPosition1;
    
    [Attribute(category: "Spawn Menu", desc: "Second spawn position")]
    protected ref PointInfo m_SpawnPosition2;
    
    string GetName()
    {
        return m_MenuName;
    }
    
    vector GetSpawnPosition1()
    {
        if (!m_SpawnPosition1)
            return vector.Zero;
            
        vector mat[4];
        m_SpawnPosition1.GetModelTransform(mat);
        return mat[3];
    }
    
    vector GetSpawnPosition2()
    {
        if (!m_SpawnPosition2)
            return vector.Zero;
            
        vector mat[4];
        m_SpawnPosition2.GetModelTransform(mat);
        return mat[3];
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