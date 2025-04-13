[BaseContainerProps()]
class HCL_ProcessResourceAction : ScriptedUserAction
{
    // Component that contains processing information
    protected HCL_ProcessableResourceComponent m_ProcessComponent;
    
    //------------------------------------------------------------------------------------------------
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        super.Init(pOwnerEntity, pManagerComponent);
        
        // Find the process component
        m_ProcessComponent = HCL_ProcessableResourceComponent.Cast(pOwnerEntity.FindComponent(HCL_ProcessableResourceComponent));
        if (!m_ProcessComponent) 
        {
            Print("Error! Could not find Processable Resource Component for this action.", LogLevel.ERROR);
        }
    }
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        if (!GetGame().InPlayMode())
            return;
            
        // Check if we have the process component
        if (!m_ProcessComponent)
            return;
            
        // Get inventory manager
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
            return;
            
        // Check if player has all required items
        if (!m_ProcessComponent.HasRequiredItems(inventoryManager))
        {
            SCR_HintManagerComponent.ShowCustomHint(m_ProcessComponent.GetRequiredItemsDescription(), "Missing Items", 3.0);
            return;
        }
        
        // Get output prefab from component
        ResourceName outputPrefab = m_ProcessComponent.GetOutputPrefab();
        if (outputPrefab.IsEmpty())
            return;
            
        // Spawn output item
        Resource outputItem = Resource.Load(outputPrefab);
        if (!outputItem)
            return;
            
        EntitySpawnParams spawnParams = new EntitySpawnParams();
        spawnParams.TransformMode = ETransformMode.WORLD;
        
        vector mat[4];
        pUserEntity.GetWorldTransform(mat);
        spawnParams.Transform = mat;
        
        IEntity outputEntity = GetGame().SpawnEntityPrefab(outputItem, GetGame().GetWorld(), spawnParams);
        if (!outputEntity)
            return;
            
        // Check if we can insert the output item
        BaseInventoryStorageComponent storage = inventoryManager.FindStorageForItem(outputEntity, EStoragePurpose.PURPOSE_ANY);
        if (!storage || !inventoryManager.CanInsertItemInStorage(outputEntity, storage))
        {
            // Delete the spawned output if we can't add it to inventory
            if (outputEntity)
            {
                RplComponent rplComp = RplComponent.Cast(outputEntity.FindComponent(RplComponent));
                if (rplComp)
                    RplComponent.DeleteRplEntity(outputEntity, false);
            }
            
            // Show hint about not having space
            SCR_HintManagerComponent.ShowCustomHint("Cannot process items. Not enough inventory space.", "Inventory Full", 3.0);
            return;
        }
        
        // Remove required items from inventory
        if (!m_ProcessComponent.RemoveRequiredItems(inventoryManager))
        {
            // Delete the spawned output if we couldn't remove required items
            if (outputEntity)
            {
                RplComponent rplComp = RplComponent.Cast(outputEntity.FindComponent(RplComponent));
                if (rplComp)
                    RplComponent.DeleteRplEntity(outputEntity, false);
            }
            
            SCR_HintManagerComponent.ShowCustomHint("Failed to remove required items.", "Processing Failed", 3.0);
            return;
        }
        
        // Add output item to inventory
        inventoryManager.InsertItem(outputEntity);
        
        // Show success hint
        SCR_HintManagerComponent.ShowCustomHint(string.Format("Created %1", m_ProcessComponent.GetOutputName()), "Processing Complete", 3.0);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {
        if (!user || !m_ProcessComponent)
            return false;
            
        // Check if user has inventory manager
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
            return false;
            
        // Check if user has all required items
        return m_ProcessComponent.HasRequiredItems(inventoryManager);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBeShownScript(IEntity user)
    {
        // Always show the action
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        if (!m_ProcessComponent)
        {
            outName = "Process Items";
            return true;
        }
        
        // Get output name from component
        string outputName = m_ProcessComponent.GetOutputName();
        outName = string.Format("Process into %1", outputName);
        
        return true;
    }
} 