using System.Collections.Generic;
using UnityEngine;

namespace Flakkari4Unity
{
    public class Synchronizer : MonoBehaviour
    {
        private readonly Dictionary<ulong, ECS.Entity> entities = new();

        public void AddEntity(ulong id, ECS.Entity entity, byte[] payload)
        {
            if (entities.ContainsKey(id))
            {
                Debug.LogWarning($"Entity with id {id} already exists.");
                return;
            }

            entity.Id = id;

            entity.CreateComponents(payload);

            entities.Add(id, entity);
        }

        public void RemoveEntity(ulong id)
        {
            if (entities.TryGetValue(id, out ECS.Entity entity))
            {
                entities.Remove(id);
                Destroy(entity.gameObject);
            }
        }

        public ECS.Entity GetEntity(ulong id)
        {
            return entities[id];
        }
    }
}
