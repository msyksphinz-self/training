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
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class lea_r16label extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "lea_r16label",
          lea_r16label.class,
          null
          );
      addArgument("op1", R16.INFO);
      addArgument("rel", SWORD);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R16 op1 = (R16) getArgument("op1", args);
      final Immediate rel = (Immediate) getArgument("rel", args);

      return new lea_r16label(op1, rel);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "lea_r16label",
          instruction.class,
          null
          );
      addArgument("op1", R16.INFO);
      addArgument("rel", SWORD);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R16 op1 = (R16) getArgument("op1", args);
      final Immediate rel = (Immediate) getArgument("rel", args);

      return new instruction(
        new lea_r16label(
          op1,
          rel
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R16 op1;
  public final Immediate rel;

  public lea_r16label(final R16 op1, final Immediate rel) {
    assert R16.INFO.isSupported(op1);
    assert SWORD.equals(rel.getType());

    this.op1 = op1;
    this.rel = rel;

    addArgument("op1", op1);
    addArgument("rel", rel);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("lea %s, [<label>%d]", op1.text(vars__), rel.access().load().bigIntegerValue());
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("10111%s%s", op1.image(vars__), rel.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    op1.access(pe__, vars__).store(Data.cast(WORD, rel.access().load()));
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x3));
  }
}