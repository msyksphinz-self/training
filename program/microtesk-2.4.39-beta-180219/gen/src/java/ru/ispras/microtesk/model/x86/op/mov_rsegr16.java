/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;
import ru.ispras.microtesk.model.x86.mode.*;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class mov_rsegr16 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "mov_rsegr16",
          mov_rsegr16.class,
          null
          );
      addArgument("dst", RSEG16.INFO);
      addArgument("src", R16.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final RSEG16 dst = (RSEG16) getArgument("dst", args);
      final R16 src = (R16) getArgument("src", args);

      return new mov_rsegr16(dst, src);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "mov_rsegr16",
          instruction.class,
          null
          );
      addArgument("dst", RSEG16.INFO);
      addArgument("src", R16.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final RSEG16 dst = (RSEG16) getArgument("dst", args);
      final R16 src = (R16) getArgument("src", args);

      return new instruction(
        new mov_rsegr16(
          dst,
          src
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final RSEG16 dst;
  public final R16 src;

  public mov_rsegr16(final RSEG16 dst, final R16 src) {
    assert RSEG16.INFO.isSupported(dst);
    assert R16.INFO.isSupported(src);

    this.dst = dst;
    this.src = src;

    addArgument("dst", dst);
    addArgument("src", src);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("mov %s, %s", dst.text(vars__), src.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("10001110110%s%s", dst.image(vars__), src.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    dst.access(pe__, vars__).store(src.access(pe__, vars__).load());
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x2));
  }
}