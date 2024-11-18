using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Synchronizer : MonoBehaviour
{
    private readonly Dictionary<ulong, GameObject> entities = new();

    public void AddEntity(ulong id, GameObject entity)
    {
        entities.Add(id, entity);
    }

    public void RemoveEntity(ulong id)
    {
        entities.Remove(id);
    }

    public GameObject GetEntity(ulong id)
    {
        return entities[id];
    }
}
