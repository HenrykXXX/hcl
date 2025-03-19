[BaseContainerProps()]
class HCL_GatherPeachAction : ScriptedUserAction
{
    // Resource path to the peach item prefab
    protected ResourceName m_PeachPrefab = "{64E0ABC569170B6A}prefabs/items/food/hcl_peach.et";
    
    // Cooldown time in milliseconds (10 seconds)
    protected float m_fCooldownTime = 10000.0;
    
    // Last time a peach was gathered - instance specific
    protected float m_fLastGatherTime = 0;
    
    //------------------------------------------------------------------------------------------------
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        super.Init(pOwnerEntity, pManagerComponent);
        // Initialize the timer to current world time
        m_fLastGatherTime = GetGame().GetWorld().GetWorldTime();
    }
	
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        if (!GetGame().InPlayMode())
            return;
            
        // Get inventory manager
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
            return;
            
        // Record current time for cooldown
        m_fLastGatherTime = GetGame().GetWorld().GetWorldTime();
            
        // Spawn peach
        Resource peachResource = Resource.Load(m_PeachPrefab);
        if (!peachResource)
            return;
            
        EntitySpawnParams spawnParams = new EntitySpawnParams();
        spawnParams.TransformMode = ETransformMode.WORLD;
        
        vector mat[4];
        pUserEntity.GetWorldTransform(mat);
        spawnParams.Transform = mat;
        
        IEntity peachEntity = GetGame().SpawnEntityPrefab(peachResource, GetGame().GetWorld(), spawnParams);
        if (!peachEntity)
            return;
        
        // Check if we can insert the item first
        BaseInventoryStorageComponent storage = inventoryManager.FindStorageForItem(peachEntity, EStoragePurpose.PURPOSE_ANY);
        if (!storage || !inventoryManager.CanInsertItemInStorage(peachEntity, storage))
        {
            // Delete the spawned peach if we can't add it to inventory
            if (peachEntity)
            {
                RplComponent rplComp = RplComponent.Cast(peachEntity.FindComponent(RplComponent));
                if (rplComp)
                    RplComponent.DeleteRplEntity(peachEntity, false);
            }
                
            return;
        }
            
        // Add to inventory
        inventoryManager.InsertItem(peachEntity);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {
        if (!user)
            return false;
            
        // Check if user has inventory manager
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
            return false;
        
        // Check cooldown
        float currentTime = GetGame().GetWorld().GetWorldTime();
        float timeRemaining = m_fLastGatherTime + m_fCooldownTime - currentTime;
        
        // Check if still on cooldown
        if (timeRemaining > 0)
            return false;
        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBeShownScript(IEntity user)
    {
        // Always show the action even during cooldown
        if (!user)
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
        // Check if action is on cooldown
        float currentTime = GetGame().GetWorld().GetWorldTime();
        if (currentTime < m_fLastGatherTime + m_fCooldownTime)
        {
            // Calculate remaining cooldown time in seconds
            int remainingTime = Math.Round((m_fLastGatherTime + m_fCooldownTime - currentTime)/1000);
            outName = string.Format("Gather Peach (Available in %1 sec)", remainingTime);
        }
        else
        {
            outName = "Gather Peach";
        }
        
        return true;
    }
}