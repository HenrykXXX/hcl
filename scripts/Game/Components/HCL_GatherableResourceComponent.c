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
    
    [RplProp(onRplName: "OnLastGatherTimeChanged")]
    protected float m_fLastGatherTime;
    
    //------------------------------------------------------------------------------------------------
    void HCL_GatherableResourceComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
    {
        m_pComponentClass = HCL_GatherableResourceComponentClass.Cast(GetComponentData(ent));
        m_fLastGatherTime = 0.0;
    }
    
    //------------------------------------------------------------------------------------------------
    protected void OnLastGatherTimeChanged()
    {
        // This will be called whenever the last gather time is changed on any machine
		Print("OnLastGatherTimeChanged");
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
    
    //------------------------------------------------------------------------------------------------
    //! Check if the resource is on cooldown
    bool IsOnCooldown()
    {
        float currentTime = GetGame().GetWorld().GetWorldTime();
        float cooldownTime = GetCooldownTime();
        return (currentTime < m_fLastGatherTime + cooldownTime);
    }
    
    //------------------------------------------------------------------------------------------------
    //! Get the remaining cooldown time in seconds
    int GetRemainingCooldownSeconds()
    {
        float currentTime = GetGame().GetWorld().GetWorldTime();
        float cooldownTime = GetCooldownTime();
        float remainingTime = m_fLastGatherTime + cooldownTime - currentTime;
        return Math.Round(remainingTime / 1000);
    }
    
    //------------------------------------------------------------------------------------------------
    //! Set the last gather time (only on server)
    void SetLastGatherTime()
    {
        if (Replication.IsServer())
        {
            m_fLastGatherTime = GetGame().GetWorld().GetWorldTime();
            Replication.BumpMe();
        }
    }
	
	protected bool m_bIsTurnedOn;					// this value is edited only on authority's side

	protected RplComponent GetRpl(RplId id)
	{
		Print("retrying...");
		return RplComponent.Cast(Replication.FindItem(id));
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_Broadcast_Method(RplId itemRplId, RplId userRplId)
	{
	    // Get the item entity from RplId
	    RplComponent itemRplComp = RplComponent.Cast(Replication.FindItem(itemRplId));
	   	if (!itemRplComp)
		{
			Print("Retrying in 100ms...");
			GetGame().GetCallqueue().CallLater(RpcDo_Broadcast_Method, 100, false, itemRplId, userRplId);
			return;
		}
		
	    IEntity itemEntity = itemRplComp.GetEntity();
	    if (!itemEntity)
	        return;
	        
	    // Get the user entity from RplId
	    RplComponent userRplComp = RplComponent.Cast(Replication.FindItem(userRplId));
	    if (!userRplComp)
	        return;
	        
	    IEntity userEntity = userRplComp.GetEntity();
	    if (!userEntity)
	        return;
	        
	    // Find inventory manager on user entity
	    SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(userEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	    if (!inventoryManager)
	        return;
	        
	    // Add item to inventory
	    inventoryManager.InsertItem(itemEntity);
	    
	    // Show success hint to the user
	    string resourceName = GetResourceName();
	    SCR_HintManagerComponent.ShowCustomHint(string.Format("Gathered %1", resourceName), "Resource Gathered", 2.0);
	}
	
	// public methods
	void AddToInventory(IEntity item, SCR_InventoryStorageManagerComponent manager)
	{
	    // Get the RplComponent for the item
	    RplComponent itemRplComp = RplComponent.Cast(item.FindComponent(RplComponent));
	    if (!itemRplComp)
	        return;
		
	    RplId itemRplId = itemRplComp.Id();
	    
	    // Get the user entity that owns the inventory manager
	    IEntity userEntity = manager.GetOwner();
	    if (!userEntity)
	        return;
	        
	    // Get the RplComponent for the user
	    RplComponent userRplComp = RplComponent.Cast(userEntity.FindComponent(RplComponent));
	    if (!userRplComp)
	        return;
	        
	    RplId userRplId = userRplComp.Id();
	    
	    // Call the RPC with RplIds instead of entities
	    Rpc(RpcDo_Broadcast_Method, itemRplId, userRplId);
	}
} 