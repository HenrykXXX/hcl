[BaseContainerProps()]
class HCL_GatherPeachAction : ScriptedUserAction
{
    // Resource path to the peach item prefab
    protected ResourceName m_PeachPrefab = "{64E34969A5EB7065}prefabs/items/food/hcl_peach.et";
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {   
        SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
        if (!playerController || pUserEntity != playerController.GetMainEntity()) 
            return;
        
        // Success - spawn the peach item
        Resource peachResource = Resource.Load(m_PeachPrefab);
        if (peachResource)
        {
            // Get position for the new peach (slightly offset from the player)
            vector playerPos = pUserEntity.GetOrigin();
            vector offset = pUserEntity.GetTransformAxis(0) * 0.5; // 0.5m in front of player
            
            // Spawn the peach
            EntitySpawnParams spawnParams = new EntitySpawnParams();
            spawnParams.TransformMode = ETransformMode.WORLD;
            spawnParams.Transform[3] = playerPos + offset;
            
            IEntity peachEntity = GetGame().SpawnEntityPrefab(peachResource, null, spawnParams);
            if (peachEntity)
            {
                // Notify player of success
                SCR_HintManagerComponent.GetInstance().ShowCustomHint("You found a peach!", "Harvest Success", 3.0);
            }
        }
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBeShownScript(IEntity user)
    {
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "Gather Peach";
        return true;
    }
} 