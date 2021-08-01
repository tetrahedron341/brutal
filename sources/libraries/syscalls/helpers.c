#include <brutal/log.h>
#include <syscalls/helpers.h>
#include <syscalls/syscalls.h>

BrResult brh_close(BrHandle handle)
{
    return br_close(&(BrCloseArgs){
        .handle = handle,
    });
}

static BrGlobalInfo *global_infos = nullptr;

BrGlobalInfo *brh_global_info(void)
{
    if (global_infos == nullptr)
    {
        auto g_map = (BrMapArgs){
            .space = BR_SPACE_SELF,
            .mem_obj = BR_MEM_OBJ_GINFO,
        };

        assert_truth(br_map(&g_map) == BR_SUCCESS);
        global_infos = (BrGlobalInfo *)g_map.vaddr;
    }

    return global_infos;
}

static BrLocalInfo *local_infos = nullptr;

BrLocalInfo *brh_local_info(void)
{
    if (global_infos == nullptr)
    {
        auto l_map = (BrMapArgs){
            .space = BR_SPACE_SELF,
            .mem_obj = BR_MEM_OBJ_LINFO,
        };

        assert_truth(br_map(&l_map) == BR_SUCCESS);
        local_infos = (BrLocalInfo *)l_map.vaddr;
    }

    return local_infos;
}
