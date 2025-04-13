[BaseContainerProps(configRoot: true)]
class HCL_ProcessableResourceComponentClass : GenericComponentClass
{
    [Attribute(desc: "Output item prefab after processing", params: "et")]
    ResourceName m_OutputPrefab;
    
    [Attribute(desc: "Items needed for processing with their amounts", params: "et")]
    ref array<ref ResourceName> m_aRequiredItems;
    
    [Attribute(desc: "Amounts of required items", params: "1")]
    ref array<int> m_aRequiredAmounts;
}

//------------------------------------------------------------------------------------------------
//! Component that defines a processable resource
class HCL_ProcessableResourceComponent : GenericComponent
{
    protected HCL_ProcessableResourceComponentClass m_pComponentClass;
    
    //------------------------------------------------------------------------------------------------
    void HCL_ProcessableResourceComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
    {
        m_pComponentClass = HCL_ProcessableResourceComponentClass.Cast(GetComponentData(ent));
    }
    
    //------------------------------------------------------------------------------------------------
    //! Get the output prefab after processing
    ResourceName GetOutputPrefab()
    {
        if (m_pComponentClass)
            return m_pComponentClass.m_OutputPrefab;
        return ResourceName.Empty;
    }
    
    //------------------------------------------------------------------------------------------------
    //! Get the name of the output item
    string GetOutputName()
    {
        ResourceName prefab = GetOutputPrefab();
        if (prefab != ResourceName.Empty)
        {
            // Load the resource
            Resource resourceItem = Resource.Load(prefab);
            if (!resourceItem)
                return "Processed Item";
                
            // Set up spawn parameters
            EntitySpawnParams spawnParams = new EntitySpawnParams();
            spawnParams.TransformMode = ETransformMode.WORLD;
            
            // Spawn the entity
            IEntity entity = GetGame().SpawnEntityPrefab(resourceItem, GetGame().GetWorld(), spawnParams);
            if (entity)
            {
                string name = entity.GetPrefabData().GetPrefabName();
                SCR_EntityHelper.DeleteEntityAndChildren(entity);
                return name;
            }
        }
        return "Processed Item";
    }
    
    //------------------------------------------------------------------------------------------------
    //! Check if player has all required items in their inventory
    bool HasRequiredItems(SCR_InventoryStorageManagerComponent inventoryManager)
    {
        if (!inventoryManager || !m_pComponentClass)
            return false;
            
        array<IEntity> items = new array<IEntity>();
        inventoryManager.GetAllItems(items, inventoryManager.GetCharacterStorage());
        
        // Create a map to count items
        map<ResourceName, int> itemCounts = new map<ResourceName, int>();
        
        // Count all items in inventory
        foreach (IEntity item : items)
        {
            ResourceName prefab = item.GetPrefabData().GetPrefabName();
            int count = 0;
            if (itemCounts.Contains(prefab))
                count = itemCounts[prefab];
            itemCounts[prefab] = count + 1;
        }
        
        // Check if we have enough of each required item
        for (int i = 0; i < m_pComponentClass.m_aRequiredItems.Count(); i++)
        {
            ResourceName requiredPrefab = m_pComponentClass.m_aRequiredItems[i];
            int requiredAmount = m_pComponentClass.m_aRequiredAmounts[i];
            
            if (!itemCounts.Contains(requiredPrefab) || itemCounts[requiredPrefab] < requiredAmount)
                return false;
        }
        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    //! Remove required items from inventory
    bool RemoveRequiredItems(SCR_InventoryStorageManagerComponent inventoryManager)
    {
        if (!inventoryManager || !m_pComponentClass)
            return false;
            
        array<IEntity> items = new array<IEntity>();
        inventoryManager.GetAllItems(items, inventoryManager.GetCharacterStorage());
        
        // Create a map to track items to remove
        map<ResourceName, int> itemsToRemove = new map<ResourceName, int>();
        for (int i = 0; i < m_pComponentClass.m_aRequiredItems.Count(); i++)
        {
            itemsToRemove[m_pComponentClass.m_aRequiredItems[i]] = m_pComponentClass.m_aRequiredAmounts[i];
        }
        
        // Remove items
        foreach (IEntity item : items)
        {
            ResourceName prefab = item.GetPrefabData().GetPrefabName();
            if (itemsToRemove.Contains(prefab))
            {
                int count = itemsToRemove[prefab];
                if (count > 0)
                {
                    if (inventoryManager.TryRemoveItemFromInventory(item))
                    {
                        count--;
                        itemsToRemove[prefab] = count;
                        if (count == 0)
                            itemsToRemove.Remove(prefab);
                    }
                }
            }
        }
        
        // Check if we removed all required items
        return itemsToRemove.Count() == 0;
    }
    
    //------------------------------------------------------------------------------------------------
    //! Get description of required items
    string GetRequiredItemsDescription()
    {
        if (!m_pComponentClass)
            return "No items required";
            
        string description = "Required items: ";
        for (int i = 0; i < m_pComponentClass.m_aRequiredItems.Count(); i++)
        {
            ResourceName prefab = m_pComponentClass.m_aRequiredItems[i];
            int amount = m_pComponentClass.m_aRequiredAmounts[i];
            
            // Load the resource
            Resource resourceItem = Resource.Load(prefab);
            if (!resourceItem)
                continue;
                
            // Set up spawn parameters
            EntitySpawnParams spawnParams = new EntitySpawnParams();
            spawnParams.TransformMode = ETransformMode.WORLD;
            
            // Spawn the entity
            IEntity entity = GetGame().SpawnEntityPrefab(resourceItem, GetGame().GetWorld(), spawnParams);
            string name = "Unknown Item";
            if (entity)
            {
                name = entity.GetPrefabData().GetPrefabName();
                SCR_EntityHelper.DeleteEntityAndChildren(entity);
            }
            
            if (i > 0)
                description += ", ";
            description += string.Format("%1x %2", amount, name);
        }
        
        return description;
    }
} 