[BaseContainerProps()]
class HCL_GatherResourceAction : ScriptedUserAction
{
    // Component that contains resource information
    protected HCL_GatherableResourceComponent m_ResourceComponent;
    
    //------------------------------------------------------------------------------------------------
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        super.Init(pOwnerEntity, pManagerComponent);
        
        // Find the resource component
        m_ResourceComponent = HCL_GatherableResourceComponent.Cast(pOwnerEntity.FindComponent(HCL_GatherableResourceComponent));
        if (!m_ResourceComponent) 
        {
            Print("Error! Could not find Gatherable Resource Component for this action.", LogLevel.ERROR);
        }
    }
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        if (!GetGame().InPlayMode())
            return;
            
        // Check if we have the resource component
        if (!m_ResourceComponent)
            return;
				
        // Get inventory manager
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
            return;
            
        // Get resource prefab from component
        ResourceName resourcePrefab = m_ResourceComponent.GetResourcePrefab();
        if (resourcePrefab.IsEmpty())
            return;
            
        // Spawn resource item
        Resource resourceItem = Resource.Load(resourcePrefab);
        if (!resourceItem)
            return;
            
        EntitySpawnParams spawnParams = new EntitySpawnParams();
        spawnParams.TransformMode = ETransformMode.WORLD;
        
        vector mat[4];
        pUserEntity.GetWorldTransform(mat);
        spawnParams.Transform = mat;
        
        IEntity resourceEntity = GetGame().SpawnEntityPrefab(resourceItem, GetGame().GetWorld(), spawnParams);
        if (!resourceEntity)
            return;
        
        // Get the resource name for hints
        string resourceName = m_ResourceComponent.GetResourceName();
        
        // Check if we can insert the item first
        BaseInventoryStorageComponent storage = inventoryManager.FindStorageForItem(resourceEntity, EStoragePurpose.PURPOSE_ANY);
        if (!storage || !inventoryManager.CanInsertItemInStorage(resourceEntity, storage))
        {
            // Delete the spawned resource if we can't add it to inventory
            if (resourceEntity)
            {
                RplComponent rplComp = RplComponent.Cast(resourceEntity.FindComponent(RplComponent));
                if (rplComp)
                    RplComponent.DeleteRplEntity(resourceEntity, false);
            }
            
            // Show hint about not having space
            SCR_HintManagerComponent.ShowCustomHint(string.Format("Cannot gather %1. Not enough inventory space.", resourceName), "Inventory Full", 3.0);
                
            return;
        }
		
		m_ResourceComponent.AddToInventory(resourceEntity, inventoryManager);
            
        // Set the last gather time on the component
        m_ResourceComponent.SetLastGatherTime();
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {
        if (!user || !m_ResourceComponent)
            return false;
            
        // Check if user has inventory manager
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
            return false;
        
        // Check cooldown using component's method
        if (m_ResourceComponent.IsOnCooldown())
            return false;
        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBeShownScript(IEntity user)
    {
        // Always show the action even during cooldown
        if (!user || !m_ResourceComponent)
            return false;
            
        // Check if user has inventory manager
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
            return false;
            
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        if (!m_ResourceComponent)
        {
            outName = "Gather Resource";
            return true;
        }
        
        // Get resource name from component
        string resourceName = m_ResourceComponent.GetResourceName();
        
        // Check if action is on cooldown using component's method
        if (m_ResourceComponent.IsOnCooldown())
        {
            int remainingTime = m_ResourceComponent.GetRemainingCooldownSeconds();
            outName = string.Format("Gather %1 (Available in %2 sec)", resourceName, remainingTime);
        }
        else
        {
            outName = string.Format("Gather %1", resourceName);
        }
        
        return true;
    }
}
