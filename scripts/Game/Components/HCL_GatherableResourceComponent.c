[BaseContainerProps(configRoot: true)]
class HCL_GatherableResourceComponentClass : GenericComponentClass
{
    [Attribute(desc: "Resource prefab to spawn when gathered", params: "et")]
    ResourceName m_ResourcePrefab;
    
    [Attribute(defvalue: "10000.0", desc: "Cooldown time in milliseconds")]
    float m_fCooldownTime;
    
    [Attribute(defvalue: "Resource", desc: "Display name for the resource")]
    string m_sResourceName;
}

//------------------------------------------------------------------------------------------------
//! Component that defines a gatherable resource on an entity
class HCL_GatherableResourceComponent : GenericComponent
{
    protected HCL_GatherableResourceComponentClass m_pComponentClass;
    
    //------------------------------------------------------------------------------------------------
    void HCL_GatherableResourceComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
    {
        m_pComponentClass = HCL_GatherableResourceComponentClass.Cast(GetComponentData(ent));
    }
    
    //------------------------------------------------------------------------------------------------
    //! Get the resource prefab to spawn when gathered
    ResourceName GetResourcePrefab()
    {
        if (m_pComponentClass)
            return m_pComponentClass.m_ResourcePrefab;
            
        return ResourceName.Empty;
    }
    
    //------------------------------------------------------------------------------------------------
    //! Get the cooldown time in milliseconds
    float GetCooldownTime()
    {
        if (m_pComponentClass)
            return m_pComponentClass.m_fCooldownTime;
            
        return 10000.0; // Default 10 seconds
    }
    
    //------------------------------------------------------------------------------------------------
    //! Get the display name for the resource
    string GetResourceName()
    {
        if (m_pComponentClass)
            return m_pComponentClass.m_sResourceName;
            
        return "Resource"; // Default name
    }
} 