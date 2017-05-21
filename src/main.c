#include <glib.h>
#include <stdio.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

typedef MonoString *(*GetString)(MonoObject *);
typedef void (*SetString)(MonoObject *, MonoString *);

static gboolean cb(gpointer user_data)
{
  printf("Callback\n");
  return TRUE;
}

int main(int argc, char **argv)
{
  GMainLoop *loop = g_main_loop_new(NULL, FALSE);
  GSource *source = g_timeout_source_new(1000);
  MonoDomain *domain = mono_jit_init(argv[0]);

  /* Open module ... */
  MonoAssembly *assembly = mono_domain_assembly_open(domain, "libmodule.dll");
  MonoImage *image = mono_assembly_get_image(assembly);
  MonoClass *moduleClass = mono_class_from_name(image, "glib.Host", "Module");

  /* ... get member function pointers ... */
  MonoMethodDesc *ctorDesc = mono_method_desc_new("glib.Host.Module:.ctor(string)", FALSE);
  MonoMethodDesc *setDesc = mono_method_desc_new("glib.Host.Module:SetString(string)", FALSE);
  MonoMethodDesc *getDesc = mono_method_desc_new("glib.Host.Module:GetString()", FALSE);
  MonoMethod *ctorMethod = mono_method_desc_search_in_class(ctorDesc, moduleClass);
  MonoMethod *setMethod = mono_method_desc_search_in_class(setDesc, moduleClass);
  MonoMethod *getMethod = mono_method_desc_search_in_class(getDesc, moduleClass);
  GetString get = mono_method_get_unmanaged_thunk(getMethod);

  /* ... instantiate a Module using default onstructor ... 
  MonoObject *module1 = mono_object_new(domain, moduleClass);
  mono_runtime_object_init(module1);
  printf("module.get(); [%s]\n", mono_string_to_utf8(get(module1))); */

  /* ... and another one using ctor taking a parameter ... */
  MonoObject *module2 = mono_object_new(domain, moduleClass);
  void *args[1];
  args[0] = mono_string_new(domain, "Mono rocks");
  mono_runtime_invoke(ctorMethod, module2, args, NULL);
  printf("module2.get(); [%s]\n", mono_string_to_utf8(get(module2)));

  /* Set up */
  g_source_attach(source, NULL);
  g_source_set_callback(source, cb, NULL, NULL);

  /* Run main loop */
  g_main_loop_run(loop);

  /* Unref and cleanup prior to exit */
  g_main_loop_unref(loop);
  mono_jit_cleanup(domain);
}
